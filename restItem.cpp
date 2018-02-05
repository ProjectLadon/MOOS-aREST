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
#include <memory>

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

bool FunctionParameter::subscribe(aREST *myself) {
    return myself->registerVar(getVariableName());
}

FunctionParameterDouble::FunctionParameterDouble(rapidjson::Document &d) {
    if (d.IsObject() && d.HasMember("name") && d.HasMember("inputVariable")) {
        this->name = d["name"].GetString();
        this->variableName = d["inputVariable"].GetString();
    } else {
        throw std::invalid_argument("Invalid function parameter JSON");
    }
}

bool FunctionParameterDouble::procMail(CMOOSMsg &msg) {
    if ((msg.GetKey() == variableName) && (msg.IsDouble())) {
        urlencode(msg.GetDouble());
        return true;
    } else {
        return false;
    }
}

FunctionParameterString::FunctionParameterString(rapidjson::Document &d) {
    if (d.IsObject() && d.HasMember("name") && d.HasMember("inputVariable")) {
        this->name = d["name"].GetString();
        this->variableName = d["inputVariable"].GetString();
    } else {
        throw std::invalid_argument("Invalid function parameter JSON");
    }
}

bool FunctionParameterString::procMail(CMOOSMsg &msg) {
    if ((msg.GetKey() == variableName) && (msg.IsString())) {
        urlencode(msg.GetString());
        return true;
    } else {
        return false;
    }
}

/// @brief Generate a RestItem object of the type specified by the given JSON
unique_ptr<RestItemBase> RestItemBase::restItemFactory(rapidjson::Document &d) {
    if (d.Accept(Configuration::instance()->getDigitalReadSchemaValidator())) {
        return unique_ptr<RestItemBase>(new DigitalRead(d));
    }
    if (d.Accept(Configuration::instance()->getDigitalWriteSchemaValidator())) {
        return unique_ptr<RestItemBase>(new DigitalWrite(d));
    }
    if (d.Accept(Configuration::instance()->getAnalogReadSchemaValidator())) {
        return unique_ptr<RestItemBase>(new AnalogRead(d));
    }
    if (d.Accept(Configuration::instance()->getAnalogWriteSchemaValidator())) {
        return unique_ptr<RestItemBase>(new AnalogWrite(d));
    }
    if (d.Accept(Configuration::instance()->getVariableSchemaValidator())) {
        return unique_ptr<RestItemBase>(new Variable(d));
    }
    if (d.Accept(Configuration::instance()->getFunctionSchemaValidator())) {
        return unique_ptr<RestItemBase>(new Function(d));
    }
    return unique_ptr<RestItemBase>();
}

DigitalRead::DigitalRead(rapidjson::Document &d) {
    mytype = "DigitalRead";
    pin = d["pin"].GetInt();
    variableName = d["variable"].GetString();
}

bool DigitalRead::poll(aREST *myself) {
    pollCount++;
    if ((pollCount - lastPoll) < Configuration::instance()->getDigitalPollPeriod()) {
        return true;
    }
    RestInterface *iface = Configuration::interface();
    string request = "/digital/" + to_string(pin) + "/";
    unique_ptr<Document> d = iface->makeRequest(request);
    if (d && d->IsObject() && d->HasMember("return_value")) {
        Value* ret = Pointer("/return_value").Get(*d);
        if (ret && ret->IsInt()) {
            int tmp = ret->GetInt();
            myself->notifyDouble(variableName, tmp);
            if (tmp) {
                val = true;
            } else {
                val = false;
            }
            lastPoll = pollCount;
            return true;
        }
    }
    return false;
}

bool DigitalRead::setMode() {
    string request = "/mode/" + to_string(pin) + "/i";
    RestInterface *iface = Configuration::interface();
    iface->makeRequest(request);
    return true;
}

bool DigitalRead::equals(RestItemBase *r) {
    if (r->myType() == mytype) {
        DigitalRead* s = reinterpret_cast<DigitalRead*>(r);
        return ((s->getPin() == pin) && (s->getVariableName() == variableName));
    } else {
        return false;
    }
}

/// @brief Generate header item for the AppCast
list<string> DigitalRead::reportHeader () {
    string header = "DI" + to_string(pin);
    list<string> ret;
    ret.push_back(header);
    return ret;
}

/// @brief Generate data item for the AppCast
list<string> DigitalRead::reportLine () {
    list<string> ret;
    ret.push_back(to_string(val));
    return ret;
}

DigitalWrite::DigitalWrite(rapidjson::Document &d) {
    mytype = "DigitalWrite";
    pin = d["pin"].GetInt();
    variableName = d["variable"].GetString();
}

bool DigitalWrite::procMail(CMOOSMsg &msg) {
    if ((msg.GetKey() == variableName) && (msg.IsDouble())) {
        if (msg.GetDouble() > 0.5) {
            val = true;
        } else {
            val = false;
        }
        RestInterface *iface = Configuration::interface();
        string request = "/digital/" + to_string(pin) + "/" + to_string(val);
        unique_ptr<Document> d = iface->makeRequest(request);
        if (d) return true;
    }
    return false;
}

bool DigitalWrite::setMode() {
    string request = "/mode/" + to_string(pin) + "/o";
    RestInterface *iface = Configuration::interface();
    iface->makeRequest(request);
    return true;
}

bool DigitalWrite::equals(RestItemBase *r) {
    if (r->myType() == mytype) {
        DigitalWrite* s = reinterpret_cast<DigitalWrite*>(r);
        return ((s->getPin() == pin) && (s->getVariableName() == variableName));
    } else {
        return false;
    }
}

/// @brief Generate header item for the AppCast
list<string> DigitalWrite::reportHeader () {
    string header = "DO" + to_string(pin);
    list<string> ret;
    ret.push_back(header);
    return ret;
}

/// @brief Generate data item for the AppCast
list<string> DigitalWrite::reportLine () {
    list<string> ret;
    ret.push_back(to_string(val));
    return ret;
}

AnalogRead::AnalogRead(rapidjson::Document &d) {
    mytype = "AnalogRead";
    pin = d["pin"].GetInt();
    variableName = d["variable"].GetString();
    if (d.HasMember("gain")) gain = d["gain"].GetFloat();
    if (d.HasMember("offset")) offset = d["offset"].GetInt();
}

bool AnalogRead::poll(aREST *myself) {
    pollCount++;
    if ((pollCount - lastPoll) < Configuration::instance()->getAnalogPollPeriod()) {
        return true;
    }
    RestInterface *iface = Configuration::interface();
    string request = "/analog/" + to_string(pin) + "/";
    unique_ptr<Document> d = iface->makeRequest(request);
    if (d && d->IsObject() && d->HasMember("return_value")) {
        Value* ret = Pointer("/return_value").Get(*d);
        if (ret && ret->IsInt()) {
            lastVal = ((ret->GetInt() + offset) * gain);
            myself->notifyDouble(variableName, lastVal);
            lastPoll = pollCount;
            return true;
        }
    }
    return false;
}

bool AnalogRead::equals(RestItemBase *r) {
    if (r->myType() == mytype) {
        AnalogRead* s = reinterpret_cast<AnalogRead*>(r);
        return ((s->getPin() == pin) && (s->getVariableName() == variableName) &&
                (s->getGain() == gain) && (s->getOffset() == offset));
    } else {
        return false;
    }
}

/// @brief Generate header item for the AppCast
list<string> AnalogRead::reportHeader () {
    string header = "AI" + to_string(pin);
    list<string> ret;
    ret.push_back(header);
    return ret;
}

/// @brief Generate data item for the AppCast
list<string> AnalogRead::reportLine () {
    list<string> ret;
    ret.push_back(to_string(lastVal));
    return ret;
}

AnalogWrite::AnalogWrite(rapidjson::Document &d) {
    mytype = "AnalogWrite";
    pin = d["pin"].GetInt();
    variableName = d["variable"].GetString();
    if (d.HasMember("gain")) gain = d["gain"].GetFloat();
    if (d.HasMember("offset")) offset = d["offset"].GetInt();
    if (d.HasMember("maxValue")) max = d["maxValue"].GetInt();
    if (d.HasMember("minValue")) min = d["minValue"].GetInt();
}

bool AnalogWrite::procMail(CMOOSMsg &msg) {
    if ((msg.GetKey() == variableName) && (msg.IsDouble())) {
        lastValue = (msg.GetDouble() + offset) * gain;
        if (lastValue > max) lastValue = max;
        if (lastValue < min) lastValue = min;
        RestInterface *iface = Configuration::interface();
        string request = "/analog/" + to_string(pin) + "/" + to_string(lastValue);
        unique_ptr<Document> d = iface->makeRequest(request);
        if (d) return true;
    }
    return false;
}

bool AnalogWrite::equals(RestItemBase *r) {
    if (r->myType() == mytype) {
        AnalogWrite* s = reinterpret_cast<AnalogWrite*>(r);
        return ((s->getPin() == pin) && (s->getVariableName() == variableName) &&
                (s->getGain() == gain) && (s->getOffset() == offset));
    } else {
        return false;
    }
}

/// @brief Generate header item for the AppCast
list<string> AnalogWrite::reportHeader () {
    string header = "AO" + to_string(pin);
    list<string> ret;
    ret.push_back(header);
    return ret;
}

/// @brief Generate data item for the AppCast
list<string> AnalogWrite::reportLine () {
    list<string> ret;
    ret.push_back(to_string(lastValue));
    return ret;
}
