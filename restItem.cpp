/************************************************************/
/*    NAME:                                               */
/*    ORGN: Project Ladon                                       */
/*    FILE: restItem.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include "aREST.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/schema.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/error/en.h"
#include <iostream>
#include <streambuf>
#include <curl/curl.h>
#include <memory>

using namespace std;
using namespace rapidjson;
using namespace RestItem;

unique_ptr<FunctionParameter> FunctionParameter::functionParameterFactory (rapidjson::Value &d) {
    //cerr << "Processing function parameter" << endl;
    if (d.IsObject() && d.HasMember("name") && d.HasMember("inputVariable") && d.HasMember("inputVariableType")) {
        string mytype = d["inputVariableType"].GetString();
        //cerr << "Creating function parameter of type " << mytype << endl;
        if (mytype == "DOUBLE") return unique_ptr<FunctionParameter>(new FunctionParameterDouble(d));
        if (mytype == "STRING") return unique_ptr<FunctionParameter>(new FunctionParameterString(d));
    }
    return unique_ptr<FunctionParameter>(nullptr);
}

bool FunctionParameter::subscribe(aREST *myself) {
    return myself->registerVar(getVariableName());
}

FunctionParameterDouble::FunctionParameterDouble(rapidjson::Value &d) {
    if (d.IsObject() && d.HasMember("name") && d.HasMember("inputVariable")) {
        this->name = d["name"].GetString();
        this->variableName = d["inputVariable"].GetString();
    } else {
        throw std::invalid_argument("Invalid function parameter JSON");
    }
}

void FunctionParameterDouble::urlencode(double val) {
    CURL *hnd = curl_easy_init();
    string encval = to_string(val);
    char *encoded = curl_easy_escape(hnd, encval.c_str(), 0);
    encodedValue = encoded;
    curl_free(encoded);
    curl_easy_cleanup(hnd);
}

bool FunctionParameterDouble::procMail(CMOOSMsg &msg) {
    if ((msg.GetKey() == variableName) && (msg.IsDouble())) {
        urlencode(msg.GetDouble());
        return true;
    } else {
        return false;
    }
}

FunctionParameterString::FunctionParameterString(rapidjson::Value &d) {
    if (d.IsObject() && d.HasMember("name") && d.HasMember("inputVariable")) {
        this->name = d["name"].GetString();
        this->variableName = d["inputVariable"].GetString();
    } else {
        throw std::invalid_argument("Invalid function parameter JSON");
    }
}

void FunctionParameterString::urlencode(std::string val) {
    CURL *hnd = curl_easy_init();
    char *encoded = curl_easy_escape(hnd, val.c_str(), 0);
    encodedValue = encoded;
    curl_free(encoded);
    curl_easy_cleanup(hnd);
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
unique_ptr<RestItemBase> RestItemBase::restItemFactory(rapidjson::Value &d) {
    if (d.IsObject() && d.HasMember("function") && d["function"].IsString()) {
        //cerr << "Provided JSON member \"function\" is a string with value \"";
        //cerr << d["function"].GetString() << "\"" << endl;
    } else {
        return unique_ptr<RestItemBase>();
    }
    string funcName = d["function"].GetString();
    //cerr << "Stored function name" << endl;

    if (funcName == "digitalRead" &&
        d.Accept(Configuration::instance()->getDigitalReadSchemaValidator())) {
            //cerr << "Creating digitalRead" << endl;
            return unique_ptr<RestItemBase>(new DigitalRead(d));
    } else if (funcName == "digitalRead") {
        cerr << "Failed to create digitalRead" << endl;
        StringBuffer sb;
        Configuration::instance()->getDigitalReadSchemaValidator().GetInvalidSchemaPointer().StringifyUriFragment(sb);
        cerr << "Invalid configuration schema: " << sb.GetString() << endl;
        cerr << "Invalid keyword: ";
        cerr << Configuration::instance()->getDigitalReadSchemaValidator().GetInvalidSchemaKeyword() << endl;
        sb.Clear();
        Configuration::instance()->getDigitalReadSchemaValidator().GetInvalidDocumentPointer().StringifyUriFragment(sb);
        cerr << "Invalid document: " << sb.GetString() << endl;
    }
    if (funcName == "digitalWrite" &&
        d.Accept(Configuration::instance()->getDigitalWriteSchemaValidator())) {
            //cerr << "Creating digitalWrite" << endl;
            return unique_ptr<RestItemBase>(new DigitalWrite(d));
    } else if (funcName == "digitalWrite") {
        cerr << "Failed to create digitalWrite" << endl;
        StringBuffer sb;
        Configuration::instance()->getDigitalWriteSchemaValidator().GetInvalidSchemaPointer().StringifyUriFragment(sb);
        cerr << "Invalid configuration schema: " << sb.GetString() << endl;
        cerr << "Invalid keyword: ";
        cerr << Configuration::instance()->getDigitalWriteSchemaValidator().GetInvalidSchemaKeyword() << endl;
        sb.Clear();
        Configuration::instance()->getDigitalWriteSchemaValidator().GetInvalidDocumentPointer().StringifyUriFragment(sb);
        cerr << "Invalid document: " << sb.GetString() << endl;
    }
    if (funcName == "analogRead" &&
        d.Accept(Configuration::instance()->getAnalogReadSchemaValidator())) {
            //cerr << "Creating analogRead" << endl;
            return unique_ptr<RestItemBase>(new AnalogRead(d));
    } else if (funcName == "analogRead") {
        cerr << "Failed to create analogRead" << endl;
        StringBuffer sb;
        Configuration::instance()->getAnalogReadSchemaValidator().GetInvalidSchemaPointer().StringifyUriFragment(sb);
        cerr << "Invalid configuration schema: " << sb.GetString() << endl;
        cerr << "Invalid keyword: ";
        cerr << Configuration::instance()->getAnalogReadSchemaValidator().GetInvalidSchemaKeyword() << endl;
        sb.Clear();
        Configuration::instance()->getAnalogReadSchemaValidator().GetInvalidDocumentPointer().StringifyUriFragment(sb);
        cerr << "Invalid document: " << sb.GetString() << endl;
    }
    if (funcName == "analogWrite" &&
        d.Accept(Configuration::instance()->getAnalogWriteSchemaValidator())) {
            //cerr << "Creating analogWrite" << endl;
            return unique_ptr<RestItemBase>(new AnalogWrite(d));
    } else if (funcName == "analogWrite") {
        cerr << "Failed to create analogWrite" << endl;
        StringBuffer sb;
        Configuration::instance()->getAnalogWriteSchemaValidator().GetInvalidSchemaPointer().StringifyUriFragment(sb);
        cerr << "Invalid configuration schema: " << sb.GetString() << endl;
        cerr << "Invalid keyword: ";
        cerr << Configuration::instance()->getAnalogWriteSchemaValidator().GetInvalidSchemaKeyword() << endl;
        sb.Clear();
        Configuration::instance()->getAnalogWriteSchemaValidator().GetInvalidDocumentPointer().StringifyUriFragment(sb);
        cerr << "Invalid document: " << sb.GetString() << endl;
    }
    if (funcName == "variable" &&
        d.Accept(Configuration::instance()->getVariableSchemaValidator())) {
            //cerr << "Creating variable" << endl;
            return unique_ptr<RestItemBase>(new Variable(d));
    } else if (funcName == "variable") {
        cerr << "Failed to create variable" << endl;
        StringBuffer sb;
        Configuration::instance()->getVariableSchemaValidator().GetInvalidSchemaPointer().StringifyUriFragment(sb);
        cerr << "Invalid configuration schema: " << sb.GetString() << endl;
        cerr << "Invalid keyword: ";
        cerr << Configuration::instance()->getVariableSchemaValidator().GetInvalidSchemaKeyword() << endl;
        sb.Clear();
        Configuration::instance()->getVariableSchemaValidator().GetInvalidDocumentPointer().StringifyUriFragment(sb);
        cerr << "Invalid document: " << sb.GetString() << endl;
    }
    if (funcName == "function" &&
        d.Accept(Configuration::instance()->getFunctionSchemaValidator())) {
            //cerr << "Creating function" << endl;
            return unique_ptr<RestItemBase>(new Function(d));
    } else if (funcName == "function") {
        cerr << "Failed to create function" << endl;
        StringBuffer sb;
        Configuration::instance()->getFunctionSchemaValidator().GetInvalidSchemaPointer().StringifyUriFragment(sb);
        cerr << "Invalid configuration schema: " << sb.GetString() << endl;
        cerr << "Invalid keyword: ";
        cerr << Configuration::instance()->getFunctionSchemaValidator().GetInvalidSchemaKeyword() << endl;
        sb.Clear();
        Configuration::instance()->getFunctionSchemaValidator().GetInvalidDocumentPointer().StringifyUriFragment(sb);
        cerr << "Invalid document: " << sb.GetString() << endl;
    }
    return unique_ptr<RestItemBase>();
}

//RestItemBase::~RestItemBase() {}

DigitalRead::DigitalRead(rapidjson::Value &d) {
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

DigitalWrite::DigitalWrite(rapidjson::Value &d) {
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

bool DigitalWrite::subscribe(aREST *myself) {
    return myself->registerVar(variableName);
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

AnalogRead::AnalogRead(rapidjson::Value &d) {
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

AnalogWrite::AnalogWrite(rapidjson::Value &d) {
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

bool AnalogWrite::subscribe(aREST *myself) {
    return myself->registerVar(variableName);
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

Variable::Variable(rapidjson::Value &d) {
    mytype = "Variable";
    variableType = d["variableType"].GetString();
    name = d["name"].GetString();
    variableName = d["variable"].GetString();
}

bool Variable::poll(aREST *myself) {
    pollCount++;
    if ((pollCount - lastPoll) < Configuration::instance()->getAnalogPollPeriod()) {
        return true;
    }
    RestInterface *iface = Configuration::interface();
    string request = "/" + name;
    unique_ptr<Document> d = iface->makeRequest(request);
    if (d && d->IsObject() && d->HasMember(name.c_str())) {
        Value* ret = Pointer(request.c_str()).Get(*d);
        if ((variableType == "STRING") && ret->IsString()) {
            stringVal = ret->GetString();
            myself->notifyString(variableName, stringVal);
        } else if ((variableType == "DOUBLE") && ret->IsNumber()) {
            doubleVal = ret->GetFloat();
            myself->notifyDouble(variableName, doubleVal);
        } else return false;
        lastPoll = pollCount;
        return true;
    }
    return false;
}

bool Variable::equals(RestItemBase *r) {
    if (r->myType() == mytype) {
        Variable* s = reinterpret_cast<Variable*>(r);
        return ((s->getName() == name) && (s->getVariableName() == variableName) &&
                (s->getVariableType() == variableType));
    } else {
        return false;
    }
}

/// @brief Generate header item for the AppCast
list<string> Variable::reportHeader () {
    list<string> ret;
    ret.push_back(name);
    return ret;
}

/// @brief Generate data item for the AppCast
list<string> Variable::reportLine () {
    list<string> ret;
    if (variableType == "STRING") {
        ret.push_back(stringVal);
    } else if (variableType == "DOUBLE") {
        ret.push_back(to_string(doubleVal));
    }
    return ret;
}

Function::Function(rapidjson::Value &d) {
    mytype = "Function";
    name = d["name"].GetString();
    if (d.HasMember("returnName") && d.HasMember("returnType")) {
        cerr << "Populating function return" << endl;
        returnName = d["returnName"].GetString();
        returnType = d["returnType"].GetString();
    }
    if (d.HasMember("parameters")) {
        cerr << "Populating function parameters" << endl;
        for (auto& v : d["parameters"].GetArray()) {
            arguments.push_back(FunctionParameter::functionParameterFactory(v));
        }
    }
}

bool Function::poll(aREST *myself) {
    pollCount++;
    if ((pollCount - lastPoll) < Configuration::instance()->getAnalogPollPeriod()) {
        return true;
    }
    RestInterface *iface = Configuration::interface();
    string request = "/" + name;
    if (arguments.size() > 0) {
        request += "?params=";
        for (auto &p : arguments) {
            request += p->geturlencoded() + "&";
        }
    }
    unique_ptr<Document> d = iface->makeRequest(request);
    if (d && d->IsObject() && d->HasMember("return_value")) {
        Value* ret = Pointer("/return_value").Get(*d);
        if ((returnType == "STRING") && ret->IsString()) {
            stringVal = ret->GetString();
            myself->notifyString(returnName, stringVal);
        } else if ((returnType == "DOUBLE") && ret->IsNumber()) {
            doubleVal = ret->GetFloat();
            myself->notifyDouble(returnName, doubleVal);
        } else return false;
        lastPoll = pollCount;
        return true;
    }
    return false;
}

bool Function::procMail(CMOOSMsg &msg) {
    bool ret = false;
    for (auto &p : arguments) {
        ret |= p->procMail(msg);
    }
    if (ret) lastPoll = 0;   // forces the next poll() call to transmit
    return ret;
}

bool Function::subscribe(aREST *myself) {
    bool ret = true;
    for (auto &p : arguments) {
        ret &= p->subscribe(myself);
    }
    return ret;
}

bool Function::equals(RestItemBase *r) {
    if (r->myType() == mytype) {
        Function* s = reinterpret_cast<Function*>(r);
        bool ret = ((s->getName() == name) &&
                    (s->getReturnName() == returnName) &&
                    (s->getReturnType() == returnType));
        if (ret && (arguments.size() == s->getArgs().size())) {
            for (int i = 0; i < arguments.size(); i++) {
                ret &= (arguments[i]->equals(s->getArgs()[i].get()));
            }
        }
        return ret;
    }
    return false;
}

/// @brief Generate header item for the AppCast
list<string> Function::reportHeader () {
    list<string> ret;
    ret.push_back(name);
    return ret;
}

/// @brief Generate data item for the AppCast
list<string> Function::reportLine () {
    list<string> ret;
    if (returnType == "STRING") {
        ret.push_back(stringVal);
    } else if (returnType == "DOUBLE") {
        ret.push_back(to_string(doubleVal));
    }
    return ret;
}
