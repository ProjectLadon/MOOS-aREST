/************************************************************/
/*    NAME:                                               */
/*    ORGN: MIT                                             */
/*    FILE: aREST.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef aREST_HEADER
#define aREST_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "rapidjson/rapidjson.h"

class aRESTItem {
	public:
		virtual aRESTItem(rapidjson::Document config);
		~aRESTItem();

		virtual bool fetchItem();

	protected:


}

class aREST : public AppCastingMOOSApp
{
 public:
   aREST();
   ~aREST();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();

 private: // Configuration variables
 	std::vector<aRESTItem*>		incomingItems;
 	std::vector<aRESTItem*>		outgoingItems;
 	bool						isSerial;
 	std::string					serialDevice;
 	int							serialSpeed;
 	std::string					url;

 private: // State variables
};

#endif 
