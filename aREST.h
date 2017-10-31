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
#include "rapidjson/document.h"
#include <cstdarg>

class aRESTItem {
	public:
		aRESTItem() {newMail = false;};
		~aRESTItem() {};

		static aRESTItem* itemFactory(rapidjson::Document& d);

		virtual bool fetchItem() = 0;
		virtual bool subscribeVariable(std::string var, ...) = 0;
		virtual bool readMail(CMOOSMsg &msg) = 0;
		virtual bool registerVars() = 0;

	protected:
		std::string fetchSerial(std::string request);
		std::string fetchURL(std::string request);
		bool newMail;
};

class DigitalRead : public aRESTItem {
	public:
		DigitalRead(rapidjson::Document& d);
		~DigitalRead();
		bool fetchItem();
		bool subscribeVariable(std::string var, ...) {return false;};
		bool readMail(CMOOSMsg &msg) {return false;};
		bool registerVars() {return false;};

	protected:
		std::string varName;
		bool state;
};

class DigitalWrite : public aRESTItem {
	public:
		DigitalWrite(rapidjson::Document& d);
		~DigitalWrite();
		bool fetchItem();
		bool subscribeVariable(std::string var, ...);
		bool readMail(CMOOSMsg &msg);
		bool registerVars();

	protected:
		std::string varName;
		bool state;
};

class AnalogRead : public aRESTItem {
	public:
		AnalogRead(rapidjson::Document& d);
		~AnalogRead();
		bool fetchItem();
		bool subscribeVariable(std::string var, ...) {return false;};
		bool readMail(CMOOSMsg &msg) {return false;};
		bool registerVars() {return false;};

	protected:
		uint16_t state;
};

class AnalogWrite : public aRESTItem {
	public:
		AnalogWrite(rapidjson::Document& d);
		~AnalogWrite();
		bool fetchItem();
		bool subscribeVariable(std::string var, ...);
		bool readMail(CMOOSMsg &msg);
		bool registerVars();

	protected:
		std::string varName;
		uint16_t state;
};

class Variable : public aRESTItem {
	public:
		Variable(rapidjson::Document& d);
		~Variable();
		bool fetchItem();
		bool subscribeVariable(std::string var, ...) {return false;};
		bool readMail(CMOOSMsg &msg) {return false;};
		bool registerVars() {return false;};

	protected:
		bool isString;
		bool isDouble;
		std::string stringResult;
		double doubleResult;
};

class Function : public aRESTItem {
	public:
		Function(rapidjson::Document& d);
		~Function();
		bool fetchItem();
		bool subscribeVariable(std::string var, ...);
		bool readMail(CMOOSMsg &msg);
		bool registerVars();

	protected:
		std::vector<std::string> vars;
		bool isString;
		bool isDouble;
		std::string stringResult;
		double doubleResult;
};

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
   bool loadInterface(std::string value);
   bool loadInterface(rapidjson::Document& d);
   bool loadConfFile(std::string value);
   bool loadItem(std::string param, std::string value);

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
 	int 						digitalPoll;
 	int 						analogPoll;
 	int 						variablePoll;

 private: // State variables
 	int 						lastDigitalPoll;
 	int 						lastAnalogPoll;
 	int 						lastVariablePoll;
};

#endif 
