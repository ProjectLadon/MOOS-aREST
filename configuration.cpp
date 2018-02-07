/************************************************************/
/*    NAME:                                               */
/*    ORGN: Project Ladon                                       */
/*    FILE: configuration.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include "aREST.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/schema.h"
#include "rapidjson/error/en.h"
#include <curl/curl.h>
#include <memory>

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
    if (d.Parse(value.c_str()).HasParseError()) {
        if ("digitalPollPeriod" == param) {
            digitalPollPeriod = atof(value.c_str());
            if (digitalPollPeriod < 0) digitalPollPeriod = 0;
        } else if ("analogPollPeriod" == param) {
            analogPollPeriod = atof(value.c_str());
            if (analogPollPeriod < 0) analogPollPeriod = 0;
        } else if ("functionPollPeriod" == param) {
            functionPollPeriod = atof(value.c_str());
            if (functionPollPeriod < 0) functionPollPeriod = 0;
        } else if ("variablePollPeriod" == param) {
            variablePollPeriod = atof(value.c_str());
            if (variablePollPeriod < 0) variablePollPeriod = 0;
        } else {
            fprintf(stderr, "\nError(offset %u): %s\n", (unsigned)d.GetErrorOffset(), GetParseError_En(d.GetParseError()));
            return false;
        }
    } else {
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

}

bool Configuration::confFileReader(std::string filename) {

    return false;
}

bool Configuration::jsonDispatch(rapidjson::Document &d) {
    return false;
}

bool Configuration::valid () {
    if (iface) return true;
    return false;
}

string Configuration::buildReport () {
    return "";
}

Configuration Configuration::myconf;
RestInterface* Configuration::iface = nullptr;
