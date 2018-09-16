/************************************************************/
/*    NAME:                                               */
/*    ORGN: Project Ladon                                       */
/*    FILE: configuration.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include "aREST.h"
#include "ACTable.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/document.h"
#include "rapidjson/schema.h"
#include "rapidjson/writer.h"
#include "rapidjson/error/en.h"
#include <curl/curl.h>
#include <memory>
#include <fstream>
#include <iostream>
#include <streambuf>

// Include the schemas
#include "schema/analogread_schema.h"
#include "schema/digitalread_schema.h"
#include "schema/function_schema.h"
#include "schema/variable_schema.h"
#include "schema/analogwrite_schema.h"
#include "schema/digitalwrite_schema.h"
#include "schema/interface_schema.h"

using namespace std;
using namespace rapidjson;
using namespace RestItem;

Configuration::Configuration () {
    // set a static...
    iface = nullptr;
    cerr << "Creating schemas..." << endl;

    // read in the strings for the schemas
    Document interfaceSchema;
    Document digitalWriteSchema;
    Document digitalReadSchema;
    Document analogWriteSchema;
    Document analogReadSchema;
    Document variableSchema;
    Document functionSchema;

    // parse the schemas
    interfaceSchema.Parse(reinterpret_cast<char*>(interface_schema_json), interface_schema_json_len);
    digitalWriteSchema.Parse(reinterpret_cast<char*>(digitalwrite_schema_json), digitalwrite_schema_json_len);
    digitalReadSchema.Parse(reinterpret_cast<char*>(digitalread_schema_json), digitalread_schema_json_len);
    analogWriteSchema.Parse(reinterpret_cast<char*>(analogwrite_schema_json), analogwrite_schema_json_len);
    analogReadSchema.Parse(reinterpret_cast<char*>(analogread_schema_json), analogread_schema_json_len);
    variableSchema.Parse(reinterpret_cast<char*>(variable_schema_json), variable_schema_json_len);
    functionSchema.Parse(reinterpret_cast<char*>(function_schema_json), function_schema_json_len);

    // Create the validators
    interfaceSchemaDocument = new SchemaDocument(interfaceSchema);
    interfaceSchemaValidator = new SchemaValidator(*interfaceSchemaDocument);
    digitalWriteSchemaDocument = new SchemaDocument(digitalWriteSchema);
    digitalWriteSchemaValidator = new SchemaValidator(*digitalWriteSchemaDocument);
    digitalReadSchemaDocument = new SchemaDocument(digitalReadSchema);
    digitalReadSchemaValidator = new SchemaValidator(*digitalReadSchemaDocument);
    analogWriteSchemaDocument = new SchemaDocument(analogWriteSchema);
    analogWriteSchemaValidator = new SchemaValidator(*analogWriteSchemaDocument);
    analogReadSchemaDocument = new SchemaDocument(analogReadSchema);
    analogReadSchemaValidator = new SchemaValidator(*analogReadSchemaDocument);
    variableSchemaDocument = new SchemaDocument(variableSchema);
    variableSchemaValidator = new SchemaValidator(*variableSchemaDocument);
    functionSchemaDocument = new SchemaDocument(functionSchema);
    functionSchemaValidator = new SchemaValidator(*functionSchemaDocument);

}

bool Configuration::populate(std::string param, std::string value) {
    Document d;
    cerr << "Parsing " << param << "=" << value << endl;
    if ("DIGITALPOLLPERIOD" == param) {
        digitalPollPeriod = atof(value.c_str());
        if (digitalPollPeriod < 0) digitalPollPeriod = 0;
    } else if ("ANALOGPOLLPERIOD" == param) {
        analogPollPeriod = atof(value.c_str());
        if (analogPollPeriod < 0) analogPollPeriod = 0;
    } else if ("FUNCTIONPOLLPERIOD" == param) {
        functionPollPeriod = atof(value.c_str());
        if (functionPollPeriod < 0) functionPollPeriod = 0;
    } else if ("VARIABLEPOLLPERIOD" == param) {
        variablePollPeriod = atof(value.c_str());
        if (variablePollPeriod < 0) variablePollPeriod = 0;
    } else if ("CONFFILE" == param) {
        return confFileReader(value);
    } else if (d.Parse(value.c_str()).HasParseError()) {
        cerr << "JSON parse error " << GetParseError_En(d.GetParseError());
        cerr << " in " << param << " at offset " << d.GetErrorOffset() << endl;
        return false;
    } else {
		cerr << "Dispatching JSON object " << value << endl;
        return jsonDispatch(d);
    }
    return false;
}

void Configuration::poll(aREST *myself) {
    for (auto &a : pollList) {
        a->poll(myself);
    }
}

bool Configuration::procMail(CMOOSMsg &msg) {
    bool ret = false;
    for (auto &a : pollList) {
        ret |= a->procMail(msg);
    }
    return ret;
}

bool Configuration::subscribe(aREST *myself) {
    bool ret = true;
    for (auto &a : pollList) {
        ret &= a->subscribe(myself);
    }
    return ret;
}

bool Configuration::setMode() {
    bool ret = true;
    for (auto &a : pollList) {
        ret &= a->setMode();
    }
    return ret;
}

void Configuration::dedupe () {
    // search for duplicates; if one is found, the first occurence is set to nullptr
    for (auto a = pollList.begin(); a != pollList.end(); a++) {
        for (auto b = a; b != pollList.end(); b++) {
            if (a->get()->equals(b->get())) {
                a->reset(nullptr);
                break;
            }
        }
    }
    // erase-remove idiom to get rid of the nulls
    pollList.erase(remove(pollList.begin(), pollList.end(), nullptr), pollList.end());
}

bool Configuration::confFileReader(std::string filename) {
    ifstream confFile;
    confFile.open(filename);
    // Make sure the file opened correctly
    if (!confFile.is_open()) {
        cerr << "Failed to open configuration file " << filename << endl;
        return false;
    }
    // Vacuum up the conf file
    string json;
    confFile.seekg(0, ios::end);
    json.reserve(confFile.tellg());
    confFile.seekg(0, ios::beg);
    json.assign((istreambuf_iterator<char>(confFile)), istreambuf_iterator<char>());
    rapidjson::Document d;
    // Parse and check for errors
    if (d.Parse(json.c_str()).HasParseError()) {
        cerr << "JSON parse error " << GetParseError_En(d.GetParseError());
        cerr << " in " << filename << " at offset " << d.GetErrorOffset() << endl;
        return false;
    }
    // Make sure it's an array
    if (!d.IsArray()) {
        cerr << "JSON read from " << filename << " is not an array." << endl;
        return false;
    }
    // Dispatch the JSON to build the configuration
    bool result = true;
    for (auto &v : d.GetArray()) {
        result &= jsonDispatch(v);
    }
    return result;
}

bool Configuration::jsonDispatch(rapidjson::Value &d) {
    if (d.IsObject() && d.HasMember("interfaceType") && d.Accept(*interfaceSchemaValidator)) {
        cerr << "Dispatching interface" << endl;
        iface = RestInterface::restInterfaceFactory(d).release();
        if (iface) return true;
    } else if (d.IsObject()) {
        cerr << "Dispatching REST item" << endl;
        auto tmp = RestItemBase::restItemFactory(d);
        if (tmp) {
            pollList.push_back(move(tmp));
            return true;
        } else {
            StringBuffer buf;
            Writer<StringBuffer> writer(buf);
            d.Accept(writer);
            cerr << "Failed to dispatch JSON :" << endl;
            cerr << "\t" << buf.GetString() << endl;
        }
    } else {
        cerr << "Provided JSON is not object and therefore not dispatchable" << endl;
    }
    return false;
}

bool Configuration::valid () {
    if (iface) return true;
    return false;
}

string Configuration::buildReport () {
    list<string> headers;
    list<string> values;
    // Assemble the headers & values as a unified list
    for (auto &a: pollList) {
        list<string> tmph = a->reportHeader();
        for (auto &h: tmph) headers.push_back(h);
        list<string> tmpv = a->reportLine();
        for (auto &v: tmpv) values.push_back(v);
    }
    // Assemble the response
    ACTable actab(headers.size());
    for (auto &h: headers) actab << h;
    actab.addHeaderLines();
    for (auto &v: values) actab << v;
    return actab.getFormattedString();
}

Configuration* Configuration::myconf = nullptr;
RestInterface* Configuration::iface = nullptr;
