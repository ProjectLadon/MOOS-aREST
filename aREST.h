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
#include "rapidjson/schema.h"
#include <cstdarg>
#include <memory>

class aREST : public AppCastingMOOSApp {
 public:
   aREST() {};
   ~aREST() {};
   bool notifyString(const std::string &var, const std::string &val) {
	   Notify(var, val);
   };
   bool notifyDouble(const std::string &var, const double &val) {
	   Notify(var, val);
   };
   bool registerVar(const std::string &var) {Register(var);};

 protected: // Standard CMOOSApp functions to overload
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload
   bool buildReport();

 protected:
   void registerVariables();

};

class RestInterface {
	public:
		RestInterface (std::string t);
		static std::unique_ptr<RestInterface> restInterfaceFactory(rapidjson::Document &d);
		virtual std::unique_ptr<rapidjson::Document> makeRequest(std::string request);
		std::string getInterfaceType() {return interfaceType;};

	private:
		std::string interfaceType;
};

class RestNetwork : public RestInterface {
	public:
		RestNetwork(rapidjson::Document &d, std::string type);
		std::unique_ptr<rapidjson::Document> makeRequest(std::string request);

	private:
		std::string url;
		static size_t writeMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
};

namespace RestItem {
	class FunctionParameter {
		public:
			static std::unique_ptr<FunctionParameter> functionParameterFactory (rapidjson::Document &d);
			virtual bool procMail(CMOOSMsg &msg) = 0;
			bool subscribe(aREST *myself);

			const std::string &getName() {return name;};
			const std::string &getVariableName() {return variableName;};
			const std::string &geturlencoded() {return encodedValue;};

		protected:
			std::string name;
			std::string variableName;
			std::string encodedValue;
	};

	class FunctionParameterDouble : public FunctionParameter {
		public:
			FunctionParameterDouble(rapidjson::Document &d);
			bool procMail(CMOOSMsg &msg);
		private:
			void urlencode(double val);
	};

	class FunctionParameterString : public FunctionParameter {
		public:
			FunctionParameterString(rapidjson::Document &d);
			bool procMail(CMOOSMsg &msg);
		private:
			void urlencode(std::string val);
	};

	class RestItemBase {
		public:
			static std::unique_ptr<RestItemBase> restItemFactory(rapidjson::Document &d);
			const std::string &myType() {return mytype;};
			virtual std::list<std::string> reportHeader();
			virtual std::list<std::string> reportLine();
			virtual bool poll(aREST *myself) = 0;
			virtual bool procMail(CMOOSMsg &msg) = 0;
			virtual bool setMode() = 0;
			virtual bool subscribe(aREST *myself) = 0;
			virtual bool equals(RestItemBase *r) = 0;

		protected:
			std::string mytype;
	};

	class DigitalRead: public RestItemBase {
		public:
			DigitalRead(rapidjson::Document &d);
			std::list<std::string> reportHeader();
			std::list<std::string> reportLine();
			bool poll(aREST *myself);
			bool procMail(CMOOSMsg &msg) {return false;};
			bool setMode();
			bool subscribe(aREST *myself) {return true;};
			bool equals(RestItemBase* r);
			long getPollCount() {return pollCount;};
			long getLastPoll() {return lastPoll;};
			long getPin() {return pin;};
			std::string getVariableName() {return variableName;};
		private:
			long pollCount = 0;
			long lastPoll = 0;
			int pin;
			bool val = false;
			std::string variableName;
	};

	class DigitalWrite: public RestItemBase {
		public:
			DigitalWrite(rapidjson::Document &d);
			std::list<std::string> reportHeader();
			std::list<std::string> reportLine();
			bool poll(aREST *myself) {return true;};
			bool procMail(CMOOSMsg &msg);
			bool setMode();
			bool subscribe(aREST *myself);
			bool equals(RestItemBase* r);
			long getPin() {return pin;};
			std::string getVariableName() {return variableName;};
		private:
			bool val = false;
			int pin;
			std::string variableName;
	};

	class AnalogRead: public RestItemBase {
		public:
			AnalogRead(rapidjson::Document &d);
			std::list<std::string> reportHeader();
			std::list<std::string> reportLine();
			bool poll(aREST *myself);
			bool procMail(CMOOSMsg &msg) {return false;};
			bool setMode() {return true;};
			bool subscribe(aREST *myself) {return true;};
			bool equals(RestItemBase* r);
			long getPollCount() {return pollCount;};
			long getLastPoll() {return lastPoll;};
			long getPin() {return pin;};
			std::string getVariableName() {return variableName;};
			double getGain() {return gain;};
			int getOffset() {return offset;};
		private:
			long pollCount = 0;
			long lastPoll = 0;
			double lastVal = 0;
			int pin;
			double gain = 1.0f;
			int offset = 0;
			std::string variableName;
	};

	class AnalogWrite: public RestItemBase {
		public:
			AnalogWrite(rapidjson::Document &d);
			std::list<std::string> reportHeader();
			std::list<std::string> reportLine();
			bool poll(aREST *myself) {return true;};
			bool procMail(CMOOSMsg &msg);
			bool setMode() {return true;};
			bool subscribe(aREST *myself);
			bool equals(RestItemBase* r);
			long getPin() {return pin;};
			std::string getVariableName() {return variableName;};
			double getGain() {return gain;};
			int getOffset() {return offset;};
			int getMax() {return max;};
			int getMin() {return min;};
		private:
			int pin;
			std::string variableName;
			int lastValue;
			double gain = 1.0f;
			int offset = 0;
			int max = 255;
			int min = 0;
	};

	class Variable: public RestItemBase {
		public:
			Variable(rapidjson::Document &d);
			std::list<std::string> reportHeader();
			std::list<std::string> reportLine();
			bool poll(aREST *myself);
			bool procMail(CMOOSMsg &msg);
			bool setMode() {return true;};
			bool subscribe(aREST *myself) {return true;};
			bool equals(RestItemBase* r);
		private:
			long pollCount = 0;
			long lastPoll = 0;
			std::string variableName;
			std::string name;
			std::string variableType;
	};

	class Function: public RestItemBase {
		public:
			Function(rapidjson::Document &d);
			std::list<std::string> reportHeader();
			std::list<std::string> reportLine();
			bool poll(aREST *myself);
			bool procMail(CMOOSMsg &msg);
			bool setMode() {return true;};
			bool subscribe(aREST *myself);
			bool equals(RestItemBase* r);
		private:
			long pollCount = 0;
			long lastPoll = 0;
			std::string name;
			std::string returnName;
			std::string returnType;
			std::list<FunctionParameter*> arguments;
	};

	class Configuration {
		public:
			static Configuration* instance() {return myconf;};
			static RestInterface* interface() {return iface;};
			const int &getDigitalPollPeriod() {return digitalPollPeriod;};
			const int &getAnalogPollPeriod() {return analogPollPeriod;};
			const int &getVariablePollPeriod() {return variablePollPeriod;};
			const int &getFunctionPollPeriod() {return functionPollPeriod;};
			rapidjson::SchemaValidator &getInterfaceSchemaValidator() {return interfaceSchemaValidator;};
			rapidjson::SchemaValidator &getDigitalWriteSchemaValidator() {return digitalWriteSchemaValidator;};
			rapidjson::SchemaValidator &getDigitalReadSchemaValidator() {return digitalReadSchemaValidator;};
			rapidjson::SchemaValidator &getAnalogWriteSchemaValidator() {return analogWriteSchemaValidator;};
			rapidjson::SchemaValidator &getAnalogReadSchemaValidator() {return analogReadSchemaValidator;};
			rapidjson::SchemaValidator &getVariableSchemaValidator() {return variableSchemaValidator;};
			rapidjson::SchemaValidator &getFunctionSchemaValidator() {return functionSchemaValidator;};
			bool populate(std::string param, std::string value);
			void poll(aREST *myself);
			bool procMail(CMOOSMsg &msg);
			bool subscribe(aREST *myself);
			bool setMode();
			void dedupe();
			bool valid();
			std::string buildReport();

			~Configuration();
		private:
			Configuration();
			bool confFileReader(std::string filename);
			bool jsonDispatch(rapidjson::Document d);

			static Configuration* myconf;
			static RestInterface* iface;
			int digitalPollPeriod = 0;
			int analogPollPeriod = 0;
			int variablePollPeriod = 0;
			int functionPollPeriod = 0;
			std::list<RestItemBase*> pollList;
			rapidjson::SchemaDocument interfaceSchemaDocument;
			rapidjson::SchemaDocument digitalWriteSchemaDocument;
			rapidjson::SchemaDocument digitalReadSchemaDocument;
			rapidjson::SchemaDocument analogWriteSchemaDocument;
			rapidjson::SchemaDocument analogReadSchemaDocument;
			rapidjson::SchemaDocument variableSchemaDocument;
			rapidjson::SchemaDocument functionSchemaDocument;
			rapidjson::SchemaValidator interfaceSchemaValidator;
			rapidjson::SchemaValidator digitalWriteSchemaValidator;
			rapidjson::SchemaValidator digitalReadSchemaValidator;
			rapidjson::SchemaValidator analogWriteSchemaValidator;
			rapidjson::SchemaValidator analogReadSchemaValidator;
			rapidjson::SchemaValidator variableSchemaValidator;
			rapidjson::SchemaValidator functionSchemaValidator;
	};
}

#endif
