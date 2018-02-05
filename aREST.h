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
			bool subscribe(CMOOSApp *myself);

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

	class RestItem {
		public:
			static std::unique_ptr<RestItem> restItemFactory(rapidjson::Document &d);
			const std::string &myType() {return mytype;};
			virtual std::list<std::string> reportHeader();
			virtual std::list<std::string> reportLine();
			virtual bool poll() = 0;
			virtual bool procMail(CMOOSMsg &msg) = 0;
			virtual bool setMode() = 0;
			virtual bool subscribe(CMOOSApp *myself) = 0;
			virtual bool equals(RestItem *r) = 0;

		protected:
			std::string mytype;
	};

	class DigitalRead: public RestItem {
		public:
			DigitalRead(rapidjson::Document &d);
			std::list<std::string> reportHeader();
			std::list<std::string> reportLine();
			bool poll();
			bool procMail(CMOOSMsg &msg);
			bool setMode();
			bool subscribe(CMOOSApp *myself) {return true;};
			bool equals(RestItem* r);
		private:
			long pollCount = 0;
			long lastPoll = 0;
			int pin;
			std::string variableName;
	};

	class DigitalWrite: public RestItem {
		public:
			DigitalWrite(rapidjson::Document &d);
			std::list<std::string> reportHeader();
			std::list<std::string> reportLine();
			bool poll() {return true;};
			bool procMail(CMOOSMsg &msg);
			bool setMode();
			bool subscribe(CMOOSApp *myself);
			bool equals(RestItem* r);
		private:
			int pin;
			std::string variableName;
	};

	class AnalogRead: public RestItem {
		public:
			AnalogRead(rapidjson::Document &d);
			std::list<std::string> reportHeader();
			std::list<std::string> reportLine();
			bool poll();
			bool procMail(CMOOSMsg &msg);
			bool setMode();
			bool subscribe(CMOOSApp *myself) {return true;};
			bool equals(RestItem* r);
		private:
			long pollCount = 0;
			long lastPoll = 0;
			int pin;
			std::string variableName;
	};

	class AnalogWrite: public RestItem {
		public:
			AnalogWrite(rapidjson::Document &d);
			std::list<std::string> reportHeader();
			std::list<std::string> reportLine();
			bool poll() {return true;};
			bool procMail(CMOOSMsg &msg);
			bool setMode();
			bool subscribe(CMOOSApp *myself);
			bool equals(RestItem* r);
		private:
			int pin;
			std::string variableName;
			int lastValue;
			double gain = 1.0f;
			int offset = 0;
			int max = 255;
			int min = 0;
	};

	class Variable: public RestItem {
		public:
			Variable(rapidjson::Document &d);
			std::list<std::string> reportHeader();
			std::list<std::string> reportLine();
			bool poll();
			bool procMail(CMOOSMsg &msg);
			bool setMode() {return true;};
			bool subscribe(CMOOSApp *myself) {return true;};
			bool equals(RestItem* r);
		private:
			long pollCount = 0;
			long lastPoll = 0;
			std::string variableName;
			std::string name;
			std::string variableType;
	};

	class Function: public RestItem {
		public:
			Function(rapidjson::Document &d);
			std::list<std::string> reportHeader();
			std::list<std::string> reportLine();
			bool poll();
			bool procMail(CMOOSMsg &msg);
			bool setMode() {return true;};
			bool subscribe(CMOOSApp *myself);
			bool equals(RestItem* r);
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
			void poll();
			bool procMail(CMOOSMsg &msg);
			bool subscribe(CMOOSApp *myself);
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
			std::list<RestItem*> pollList;
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

class aREST : public AppCastingMOOSApp {
 public:
   aREST() {};
   ~aREST() {};

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

#endif
