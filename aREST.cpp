/************************************************************/
/*    NAME:                                               */
/*    ORGN: MIT                                             */
/*    FILE: aREST.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "aREST.h"

// Include the schemas
#include "schema/analogread_schema.h"
#include "schema/digitalread_schema.h"
#include "schema/function_schema.h"
#include "schema/variable_schema.h"
#include "schema/analogwrite_schema.h"
#include "schema/digitalwrite_schema.h"
#include "schema/interface_schema.h"

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/schema.h"
#include <curl/curl.h>

using namespace std;
using namespace rapidjson;

//---------------------------------------------------------
// Constructor

aREST::aREST() {

  isSerial        = false;
  serialDevice    = "";
  serialSpeed     = -1;
  url             = "";
  digitalPoll     = -1;
  analogPoll      = -1;
  variablePoll    = -1;

}

//---------------------------------------------------------
// Destructor

aREST::~aREST() {
  for (auto a : incomingItems) {delete a;};
  for (auto a : outgoingItems) {delete a;};
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool aREST::OnNewMail(MOOSMSG_LIST &NewMail) {
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key    = msg.GetKey();

    for (auto a : this->incomingItems) {
      a->readMail(msg);
    }

    if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
      reportRunWarning("Unhandled Mail: " + key);
    }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool aREST::OnConnectToServer() {
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool aREST::Iterate()
{
  AppCastingMOOSApp::Iterate();
  // Do your thing here!
  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool aREST::OnStartUp() {
  AppCastingMOOSApp::OnStartUp();

  STRING_LIST sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(!m_MissionReader.GetConfiguration(GetAppName(), sParams))
    reportConfigWarning("No config block found for " + GetAppName());

  STRING_LIST::iterator p;
  for(p=sParams.begin(); p!=sParams.end(); p++) {
    string orig  = *p;
    string line  = *p;
    string param = toupper(biteStringX(line, '='));
    string value = line;

    bool handled = false;
    if(param == "INTERFACE") {
      handled = loadInterface(value);
    } else if (param == "CONFFILE") {
      handled = loadConfFile(value);
    } else if (param == "DIGITALPOLLPERIOD") {
      digitalPoll = atof(value.c_str());
      handled = true;
    } else if (param == "ANALOGPOLLPERIOD") {
      analogPoll = atof(value.c_str());
      handled = true;
    } else if (param == "VARIABLEPOLLPERIOD") {
      variablePoll = atof(value.c_str());
      handled = true;
    } else {
      handled = loadItem(param, value);
    }

    if(!handled)
      reportUnhandledConfigWarning(orig);

  }
  
  registerVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void aREST::registerVariables() {
  for (auto a : this->incomingItems) {
    a->registerVars();
  }
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool aREST::buildReport() 
{
  m_msgs << "============================================ \n";
  m_msgs << "File:                                        \n";
  m_msgs << "============================================ \n";

  ACTable actab(4);
  actab << "Alpha | Bravo | Charlie | Delta";
  actab.addHeaderLines();
  actab << "one" << "two" << "three" << "four";
  m_msgs << actab.getFormattedString();

  return(true);
}

//------------------------------------------------------------
// Procedure: loadInterface()

bool aREST::loadInterface(std::string value) {
  string schema_str;
  Document interface, sd;

  if (interface.Parse(value.c_str()).HasParseError()) {
    return false;
  }
  schema_str.assign(reinterpret_cast<const char*>(interface_schema_json), 
                    interface_schema_json_len);
  if (sd.Parse(schema_str.c_str()).HasParseError()) {
    return false;
  }
  SchemaDocument schema(sd);
  SchemaValidator validator(schema);
  if (!interface.Accept(validator)) {
    return false;
  }

  

  return false;
}

bool aREST::loadInterface(rapidjson::Document& d) {

  return false;
}

//------------------------------------------------------------
// Procedure: loadConfFile()

bool aREST::loadConfFile(std::string value) {

  return false;
}

//------------------------------------------------------------
// Procedure: loadItem()

bool aREST::loadItem(std::string param, std::string value) {

  return false;
}



