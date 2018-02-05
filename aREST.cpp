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

using namespace std;
using namespace RestItem;

//---------------------------------------------------------
// Procedure: OnNewMail

bool aREST::OnNewMail(MOOSMSG_LIST &NewMail) {
    AppCastingMOOSApp::OnNewMail(NewMail);

    MOOSMSG_LIST::iterator p;
    for(p=NewMail.begin(); p!=NewMail.end(); p++) {
        CMOOSMsg &msg = *p;
        if (!Configuration::instance()->procMail(msg)) {
            if (msg.GetKey() != "APPCAST_REQ") {
                reportRunWarning("Unhandled Mail: " + msg.GetKey());
            }
        }
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

bool aREST::Iterate() {
    AppCastingMOOSApp::Iterate();
    Configuration::instance()->poll();
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
        string line  = *p;
        string param = toupper(biteStringX(line, '='));
        string value = line;

        if(!(Configuration::instance()->populate(param, value))) {
            reportUnhandledConfigWarning(*p);
        }
    }

    registerVariables();
    return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void aREST::registerVariables() {
  Configuration::instance()->subscribe(this);
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool aREST::buildReport() {
    m_msgs << Configuration::instance()->buildReport();
    return(true);
}
