/************************************************************/
/*    NAME:                                               */
/*    ORGN: MIT                                             */
/*    FILE: restItem.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include "aREST.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/schema.h"
#include <curl/curl.h>

using namespace std;
using namespace rapidjson;
using namespace RestItem;

unique_ptr<FunctionParameter> FunctionParameter::functionParameterFactory (rapidjson::Document &d) {
    if (d.IsObject() && d.HasMember("name") && d.HasMember("inputVariable") && d.HasMember("inputVariableType")) {
        string mytype(d["interfaceType"].GetString(), d["interfaceType"].GetStringLength());
        if (mytype == "DOUBLE") return unique_ptr<FunctionParameter>(new FunctionParameterDouble(d));
        if (mytype == "STRING") return unique_ptr<FunctionParameter>(new FunctionParameterString(d));
    }
    return unique_ptr<FunctionParameter>(nullptr);
}

bool FunctionParameter::subscribe(MOOSApp *myself) {
    return myself->Register(getVariableName());
}

FunctionParameterDouble::FunctionParameterDouble(rapidjson::Document &d) {
    if (d.IsObject() && d.HasMember("name") && d.HasMember("inputVariable")) {
        name = d["name"].GetString();
        inputVariable = d["inputVariable"].GetString();
    } else {
        throw std::invalid_argument("Invalid function parameter JSON");
    }
}

bool FunctionParameterDouble::procMail(CMOOSMsg &msg) {
    if ((msg.GetKey() == variableName) && (msg.IsDouble())) {
        encodedValue = urlencode(msg.GetDouble());
        return true;
    } else {
        return false;
    }
}

FunctionParameterString::FunctionParameterString(rapidjson::Document &d) {
    if (d.IsObject() && d.HasMember("name") && d.HasMember("inputVariable")) {
        name = d["name"].GetString();
        inputVariable = d["inputVariable"].GetString();
    } else {
        throw std::invalid_argument("Invalid function parameter JSON");
    }
}

bool FunctionParameterString::procMail(CMOOSMsg &msg) {
    if ((msg.GetKey() == variableName) && (msg.IsDouble())) {
        encodedValue = urlencode(msg.GetString());
        return true;
    } else {
        return false;
    }
}

unique_ptr<RestItem> RestItem::restItemFactory(rapidjson::Document &d) {
    if (d.Accept(Configuration::instance()->getDigitalReadSchemaValidator())) {
        return unique_ptr<RestItem>(new DigitalRead(d));
    }
    if (d.Accept(Configuration::instance()->getDigitalWriteSchemaValidator())) {
        return unique_ptr<RestItem>(new DigitalWrite(d));
    }
    if (d.Accept(Configuration::instance()->getAnalogReadSchemaValidator())) {
        return unique_ptr<RestItem>(new AnalogRead(d));
    }
    if (d.Accept(Configuration::instance()->getAnalogWriteSchemaValidator())) {
        return unique_ptr<RestItem>(new AnalogWrite(d));
    }
    if (d.Accept(Configuration::instance()->getVariableSchemaValidator())) {
        return unique_ptr<RestItem>(new Variable(d));
    }
    if (d.Accept(Configuration::instance()->getFunctionSchemaValidator())) {
        return unique_ptr<RestItem>(new Function(d));
    }
    return unique_ptr<RestItem>(nullptr);
}

DigitalRead::DigitalRead(rapidjson::Document &d) {
    mytype = "DigitalRead";
    pin = d["pin"].GetInteger();
    variableName = d["variable"].GetString();
}

bool DigitalRead::poll() {
    if ((pollCount - lastPollCount) < Configuration::instance()->getDigitalPollPeriod()) {
        return true;
    }
    RestInterace *iface = Configuration::interface();
    string request = "/digital/" + to_string(pin) + "/";
    unique_ptr<Document> d = iface->makeRequest(request);
    if (d) {

    } else {
        return false;
    }
}
