# 1.0 SDD001 {#SDD001 }

Dependencies & Build Configuration

## 1.1 SDD017 {#SDD017 }

This software **shall** be implemented as a MOOS Appcasting App as describe in the [MOOS-IvP wiki](http://oceanai.mit.edu/moos-ivp/pmwiki/pmwiki.php).

## 1.2 SDD018 {#SDD018 }

This software **shall** use [rapidjson](http://www.rapidjson.org) as the JSON parse/unparse library.

## 1.3 SDD019 {#SDD019 }

This software **shall** use libcurl as the HTTP client.

## 1.4 SDD020 {#SDD020 }

This software **shall** be compiled using cmake.

## 1.5 SDD021 {#SDD021 }

Unit tests **shall** be written using the gtest suite.

## 1.6 SDD022 {#SDD022 }

The cmake list **shall** generate a MOOS executable target.

## 1.7 SDD023 {#SDD023 }

The cmake list **shall** generate a gtest target called ```unit_tests```.

## 1.8 SDD024 {#SDD024 }

The cmake list **shall** generate a target that compiles and runs the tests called ```test```.

# 2.0 Data Types {#SDD002 }

## 2.1 ```RestInterface``` virtual base class {#SDD025 }

### 2.1.1 SDD026 {#SDD026 }

The base class **shall** be virtual.

### 2.1.2 This class **shall** define an enum ```ifaceType_t``` that contains an entry for each sub-class. {#SDD027 }

#### 2.1.2.1 SDD028 {#SDD028 }

```Base``` -- the default value

#### 2.1.2.2 SDD029 {#SDD029 }

```RestNetwork``` -- value indicating an HTTP interface

#### 2.1.2.3 SDD030 {#SDD030 }

```RestSerial``` -- value indicating a serial interface

### 2.1.3 Members {#SDD031 }

#### 2.1.3.1 ```interfaceType``` {#SDD032 }

##### 2.1.3.1.1 SDD033 {#SDD033 }

Type shall be ```ifaceType_t```.

##### 2.1.3.1.2 SDD034 {#SDD034 }

Defaults to ```Base```.

##### 2.1.3.1.3 SDD035 {#SDD035 }

Private and written only at construction.

### 2.1.4 Methods {#SDD036 }

#### 2.1.4.1 ```ifaceType_t getInterfaceType()``` {#SDD037 }

##### 2.1.4.1.1 SDD038 {#SDD038 }

Returns value of ```interfaceType```.

#### 2.1.4.2 ```std::unique_ptr<rapidjson::Document> makeRequest(string request)``` {#SDD039 }

##### 2.1.4.2.1 SDD040 {#SDD040 }

This **shall** execute the aRest request using the provided request string

##### 2.1.4.2.2 SDD041 {#SDD041 }

Returns the parsed JSON response.

##### 2.1.4.2.3 SDD042 {#SDD042 }

Returns an empty JSON document if there is no response.

#### 2.1.4.3 ```static RestInterface* factory(rapidjson::Document d)``` {#SDD043 }

##### 2.1.4.3.1 SDD044 {#SDD044 }

Takes a JSON document complying with the schema defined in schema/interface_schema.json.

##### 2.1.4.3.2 SDD045 {#SDD045 }

Returns a pointer to a new ```RestInterface``` object defined by ```d```.

##### 2.1.4.3.3 SDD046 {#SDD046 }

Returns a null pointer if ```d``` does not comply with the schema.

### 2.1.5 Constructor(s) {#SDD047 }

#### 2.1.5.1 ```RestInterface (string t)``` {#SDD048 }

##### 2.1.5.1.1 SDD049 {#SDD049 }

Parses value of t into the enumerated elements of ifaceType_t.

## 2.2 ```RestNetwork``` class {#SDD050 }

### 2.2.1 SDD051 {#SDD051 }

Sub-class of ```RestInterface```

### 2.2.2 Members {#SDD052 }

#### 2.2.2.1 ```url``` {#SDD053 }

##### 2.2.2.1.1 SDD054 {#SDD054 }

Type ```std::string```

##### 2.2.2.1.2 SDD055 {#SDD055 }

Private

##### 2.2.2.1.3 SDD056 {#SDD056 }

Names the target URL to be used by requests made using this interface.

##### 2.2.2.1.4 SDD057 {#SDD057 }

Written only at construction

### 2.2.3 Methods {#SDD058 }

#### 2.2.3.1 SDD059 {#SDD059 }

##### 2.2.3.1.1 SDD060 {#SDD060 }

Cocatenates ```url``` and ```request``` and uses libcurl to transmit the request.

##### 2.2.3.1.2 SDD061 {#SDD061 }

If the response is good (HTTP 200-series code & valid JSON), it **shall** the response as a parsed JSON document.

##### 2.2.3.1.3 SDD062 {#SDD062 }

If the response is empty, it **shall** return an empty JSON document.

##### 2.2.3.1.4 SDD063 {#SDD063 }

If there is an error, it will return the error as a JSON-encoded string.

### 2.2.4 Constructor(s) {#SDD064 }

#### 2.2.4.1 ```RestNetwork(rapidjson::Document d, std::string interfaceType)``` {#SDD065 }

##### 2.2.4.1.1 SDD066 {#SDD066 }

This constructor **shall** take a JSON object ```d``` conforming to schema/interface_schema.json and populate the members of this object.

##### 2.2.4.1.2 SDD067 {#SDD067 }

Throws a ```std::invalid_argument``` exception if ```d``` does not comply with the schema.

##### 2.2.4.1.3 SDD068 {#SDD068 }

Throws a ```std::invalid_argument``` exception if the interface type in ```d``` does not match ```interfaceType```.

## 2.3 ```RestSerial``` {#SDD069 }

### 2.3.1 SDD070 {#SDD070 }

This class **shall** be left as a stub only at this time.

## 2.4 ```RestItem``` namespace {#SDD071 }

### 2.4.1 SDD072 {#SDD072 }

The purpose of this namespace is to allow the use of class names that would otherwise be reserved and/or confusing.

### 2.4.2 SDD005 {#SDD005 }

Each sub-class of ```RestItem::RestItem``` **shall** have a member for each item in the corresponding JSON.

### 2.4.3 ```RestItem::FunctionParameter<T>``` template class {#SDD170 }

#### 2.4.3.1 SDD171 {#SDD171 }

This class stores the parameters of an input variable for the ```RestItem::Function``` class.

#### 2.4.3.2 SDD172 {#SDD172 }

This class **shall** be a template class.

#### 2.4.3.3 SDD173 {#SDD173 }

This class **shall** be specialized for ```bool```, ```double```, and ```std::string```.

#### 2.4.3.4 Private Members {#SDD174 }

##### 2.4.3.4.1 SDD175 {#SDD175 }

```std::string name``` **shall** store the parameter name.

##### 2.4.3.4.2 SDD176 {#SDD176 }

```std::string variableName``` **shall** store the variable name this parameter subscribes to.

##### 2.4.3.4.3 SDD177 {#SDD177 }

```std::string encodedValue``` **shall** store the incoming variable value URL encodede for transmission.

#### 2.4.3.5 Private Methods {#SDD178 }

##### 2.4.3.5.1 SDD179 {#SDD179 }

```void encode(<T> val)``` **shall** URL encode ```val``` and store it in ```encodedValue```. This function **must** be explicitly implemented for each specialization, namely ```bool```, ```double```, and ```std::string```.

#### 2.4.3.6 Public Methods {#SDD180 }

##### 2.4.3.6.1 SDD181 {#SDD181 }

This class **shall** define a const reader method for each private member.

##### 2.4.3.6.2 ```bool procMail(CMOOSMsg &msg)``` {#SDD182 }

###### 2.4.3.6.2.1 SDD183 {#SDD183 }

This method **must** be explicitly implemented for each specialization, namely ```bool```, ```double```, and ```std::string```.

###### 2.4.3.6.2.2 SDD184 {#SDD184 }

If ```msg.GetKey()``` is not equal to ```variableName```, this method **shall** return ```false```. This comparison **shall** be case-insensitive.

###### 2.4.3.6.2.3 SDD185 {#SDD185 }

If it is equal, then this method **shall** determine whether the message type matches the specialization of this method. If it is anything else, it **shall** return ```false```.

###### 2.4.3.6.2.4 SDD186 {#SDD186 }

If the earlier conditions pass, this method **shall** pass the incoming value to ```encode()``` and return ```true```.

##### 2.4.3.6.3 SDD187 {#SDD187 }

```std::string geturlencoded()``` **shall** return ```encodedValue```.

##### 2.4.3.6.4 SDD188 {#SDD188 }

```bool subscribe()``` **shall** register for the MOOS variable named in ```variableName``` and return ```true``` if successful.

##### 2.4.3.6.5 ```static RestItem::FunctionParameter* functionParameterFactory (rapidjson::Document d)``` {#SDD189 }

###### 2.4.3.6.5.1 SDD190 {#SDD190 }

This factory method **shall** construct a ```FunctionParameter``` object based on the specialization specified by ```inputVariableType``` and return a pointer to it.

#### 2.4.3.7 Constructor {#SDD191 }

##### 2.4.3.7.1 SDD192 {#SDD192 }

The constructor(s) of this class **shall** be private.

##### 2.4.3.7.2 ```FunctionParameter<T>(std::string name, std::string varName)``` {#SDD193 }

###### 2.4.3.7.2.1 SDD194 {#SDD194 }

This constructor **shall** populate ```name``` and ```variableName``` from the corresponding arguments.

### 2.4.4 ```RestItem::RestItem``` virtual base class {#SDD073 }

#### 2.4.4.1 Public Methods {#SDD074 }

##### 2.4.4.1.1 SDD075 {#SDD075 }

```std::string myType()```

###### 2.4.4.1.1.1 SDD195 {#SDD195 }

Return the value of the ```mytype``` argument passed in at construction.

##### 2.4.4.1.2 SDD196 {#SDD196 }

```virtual std::list<std::string> reportHeader()``` **shall** return a list of strings naming this item and its elements, starting with ```"Type"```.

##### 2.4.4.1.3 SDD197 {#SDD197 }

```virtual std::list<std::string> reportLine()``` **shall** return a list of strings with the value of each item named in ```reportHeader()```.

##### 2.4.4.1.4 SDD076 {#SDD076 }

```static RestItem* factory(rapidjson::Document d)```

###### 2.4.4.1.4.1 SDD198 {#SDD198 }

This method **shall** parse the incoming JSON object. If it conforms to one of the stored schemas, it **shall** build an object of the corresponding sub-class and return a pointer to it.

###### 2.4.4.1.4.2 SDD199 {#SDD199 }

If the construction fails or the JSON does not conform to one of the schemas, this method **shall** return ```nullptr```.

##### 2.4.4.1.5 SDD077 {#SDD077 }

```virtual bool poll()```

###### 2.4.4.1.5.1 SDD200 {#SDD200 }

This method **shall** be pure virtual.

###### 2.4.4.1.5.2 SDD201 {#SDD201 }

This method **shall** poll the aREST target, if appropriate. See implementation requirements for details.

##### 2.4.4.1.6 SDD078 {#SDD078 }

```virtual bool procMail(CMOOSMsg &msg)```

###### 2.4.4.1.6.1 SDD202 {#SDD202 }

This method **shall** be pure virtual.

###### 2.4.4.1.6.2 SDD203 {#SDD203 }

This method **shall** look for new mail and write to the aREST target if appropriate. See implementation requirements for details.

##### 2.4.4.1.7 ```virtual bool setMode()``` {#SDD204 }

###### 2.4.4.1.7.1 SDD205 {#SDD205 }

This method **shall** be pure virtual.

###### 2.4.4.1.7.2 SDD206 {#SDD206 }

This method **shall** set the mode of the aREST pin.

##### 2.4.4.1.8 ```virtual bool subscribe()``` {#SDD207 }

###### 2.4.4.1.8.1 SDD208 {#SDD208 }

This method **shall** be pure virtual.

###### 2.4.4.1.8.2 SDD209 {#SDD209 }

This method **shall** subscribe to any necessary variables.

##### 2.4.4.1.9 SDD079 {#SDD079 }

```virtual bool equals(RestItem* r)```

###### 2.4.4.1.9.1 SDD210 {#SDD210 }

This method **shall** be pure virtual.

###### 2.4.4.1.9.2 SDD211 {#SDD211 }

This method **shall** be implemented such that it returns ```true``` if and only if the argument is to an object with the same sub-class as the calling object and all data items are equal.

#### 2.4.4.2 Static Data Members {#SDD212 }

##### 2.4.4.2.1 SDD213 {#SDD213 }

```const std::vector<std::string> typeList``` **shall** store a list of the valid subclasses of ```RestItem```.

#### 2.4.4.3 Constructor {#SDD080 }

##### 2.4.4.3.1 SDD081 {#SDD081 }

```RestItem(std::string mytype)```

###### 2.4.4.3.1.1 SDD214 {#SDD214 }

Store the value of ```mytype``` for future retrieval.

###### 2.4.4.3.1.2 SDD215 {#SDD215 }

```mytype``` **shall** be equal to a member of ```typeList```. This comparison **shall** be case insensitive.

###### 2.4.4.3.1.3 SDD216 {#SDD216 }

If ```mytype``` is not equal to a member of ```typeList```, this constructor **shall** throw a ```std::invalid_argument``` exception.

### 2.4.5 SDD082 {#SDD082 }

```RestItem::DigitalRead``` class

#### 2.4.5.1 Private Members {#SDD217 }

##### 2.4.5.1.1 ```long pollCount``` {#SDD218 }

###### 2.4.5.1.1.1 SDD219 {#SDD219 }

Initialized to zero

###### 2.4.5.1.1.2 SDD220 {#SDD220 }

Incremented each time the ```poll()``` method is called.

##### 2.4.5.1.2 ```long lastPoll``` {#SDD221 }

###### 2.4.5.1.2.1 SDD222 {#SDD222 }

Initialized to zero

###### 2.4.5.1.2.2 SDD223 {#SDD223 }

Set equal to ```pollCount``` every time ```poll()``` successfully reads the target pin.

##### 2.4.5.1.3 SDD224 {#SDD224 }

```int pin``` **shall** store the value in the ```pin``` element of the configuration JSON.

##### 2.4.5.1.4 SDD225 {#SDD225 }

```std::string variableName``` **shall** store the value in the ```variable``` element of the configuration JSON.

#### 2.4.5.2 Public Methods {#SDD226 }

##### 2.4.5.2.1 ```bool poll()``` {#SDD227 }

###### 2.4.5.2.1.1 SDD228 {#SDD228 }

This method **shall** first calculate the number of times it has been called since the last time it made a successful aREST request. If this value is less than ```Configuration::digitalPollPeriod```, this method **shall** increment ```pollCount``` and return ```true```.

###### 2.4.5.2.1.2 SDD229 {#SDD229 }

This method **shall** construct and execute an aREST request of the form ```/digital/<pin>```.

###### 2.4.5.2.1.3 SDD230 {#SDD230 }

If the request is successful, this method **shall** publish the returned value to the variable named in ```variableName```, increment ```pollCount```, set ```lastPoll``` equal to ```pollCount```, and return ```true```. Otherwise, it **shall** return ```false```.

##### 2.4.5.2.2 SDD231 {#SDD231 }

```bool procMail(CMOOSMsg &msg)``` **shall** return ```false``` with no other effect.

##### 2.4.5.2.3 SDD232 {#SDD232 }

```bool setMode()``` **shall** construct and execute an aREST request of the form ```/mode/<pin>/i``` and return ```true``` if successful. Otherwise, it **shall** return false.

##### 2.4.5.2.4 SDD233 {#SDD233 }

```bool subscribe()``` **shall** return ```true``` with no other effect.

#### 2.4.5.3 Constructor {#SDD234 }

##### 2.4.5.3.1 ```DigitalRead(rapidjson::Document d)``` {#SDD235 }

###### 2.4.5.3.1.1 SDD236 {#SDD236 }

This constructor **shall** confirm that ```d``` conforms to schema/digitalread_schema.json. If it does not, this constructor **shall** throw a ```std::invalid_argument``` exception.

###### 2.4.5.3.1.2 SDD237 {#SDD237 }

This constructor **shall** initialize all data members as described.

### 2.4.6 SDD083 {#SDD083 }

```RestItem::DigitalWrite``` class

#### 2.4.6.1 Private Members {#SDD238 }

##### 2.4.6.1.1 SDD239 {#SDD239 }

```int pin``` **shall** store the value in the ```pin``` element of the configuration JSON.

##### 2.4.6.1.2 SDD240 {#SDD240 }

```std::string variableName``` **shall** store the value in the ```variable``` element of the configuration JSON.

##### 2.4.6.1.3 SDD241 {#SDD241 }

```bool lastValue``` **shall** store the last value successfully written to the aREST target.

#### 2.4.6.2 Public Methods {#SDD242 }

##### 2.4.6.2.1 SDD243 {#SDD243 }

```bool poll()``` **shall** return ```true``` with no other effect.

##### 2.4.6.2.2 ```bool procMail(CMOOSMsg &msg)``` {#SDD244 }

###### 2.4.6.2.2.1 SDD245 {#SDD245 }

If ```msg.GetKey()``` is not equal to ```variableName```, this method **shall** return ```false```. This comparison **shall** be case-insensitive.

###### 2.4.6.2.2.2 SDD246 {#SDD246 }

If it is equal, then this method **shall** determine whether the message type is BINARY. If it is anything else, it **shall** return ```false```.

###### 2.4.6.2.2.3 SDD247 {#SDD247 }

If the earlier conditions pass, this method shall construct and execute an aREST request of the form ```/digital/<pin>/<value>```, where ```<value>``` shall 1 for ```true``` and 0 for ```false```.

###### 2.4.6.2.2.4 SDD248 {#SDD248 }

This method **shall** return the result of the aREST request.

##### 2.4.6.2.3 SDD249 {#SDD249 }

```bool setMode()``` **shall** construct and execute an aREST request of the form ```/mode/<pin>/o``` and return ```true``` if successful. Otherwise, it **shall** return false.

##### 2.4.6.2.4 SDD250 {#SDD250 }

```bool subscribe()``` **shall** subscribe to the variable named by ```variableName```.

#### 2.4.6.3 Constructor {#SDD251 }

##### 2.4.6.3.1 ```DigitalWrite(rapidjson::Document d)``` {#SDD252 }

###### 2.4.6.3.1.1 SDD253 {#SDD253 }

This constructor **shall** confirm that ```d``` conforms to schema/digitalwrite_schema.json. If it does not, this constructor **shall** throw a ```std::invalid_argument``` exception.

###### 2.4.6.3.1.2 SDD254 {#SDD254 }

This constructor **shall** initialize all data members as described.

### 2.4.7 SDD084 {#SDD084 }

```RestItem::AnalogRead``` class

#### 2.4.7.1 Private Members {#SDD255 }

##### 2.4.7.1.1 ```long pollCount``` {#SDD256 }

###### 2.4.7.1.1.1 SDD257 {#SDD257 }

Initialized to zero

###### 2.4.7.1.1.2 SDD258 {#SDD258 }

Incremented each time the ```poll()``` method is called.

##### 2.4.7.1.2 ```long lastPoll``` {#SDD259 }

###### 2.4.7.1.2.1 SDD260 {#SDD260 }

Initialized to zero

###### 2.4.7.1.2.2 SDD261 {#SDD261 }

Set equal to ```pollCount``` every time ```poll()``` successfully reads the target pin.

##### 2.4.7.1.3 SDD262 {#SDD262 }

```int pin``` **shall** store the value in the ```pin``` element of the configuration JSON.

##### 2.4.7.1.4 SDD263 {#SDD263 }

```std::string variableName``` **shall** store the value in the ```variable``` element of the configuration JSON.

#### 2.4.7.2 Public Methods {#SDD264 }

##### 2.4.7.2.1 ```bool poll()``` {#SDD265 }

###### 2.4.7.2.1.1 SDD266 {#SDD266 }

This method **shall** first calculate the number of times it has been called since the last time it made a successful aREST request. If this value is less than ```Configuration::analogPollPeriod```, this method **shall** increment ```pollCount``` and return ```true```.

###### 2.4.7.2.1.2 SDD267 {#SDD267 }

This method **shall** construct and execute an aREST request of the form ```/analog/<pin>```.

###### 2.4.7.2.1.3 SDD268 {#SDD268 }

If the request is successful, this method **shall** publish the returned value to the variable named in ```variableName```, increment ```pollCount```, set ```lastPoll``` equal to ```pollCount```, and return ```true```. Otherwise, it **shall** return ```false```.

##### 2.4.7.2.2 SDD269 {#SDD269 }

```bool procMail(CMOOSMsg &msg)``` **shall** return ```false``` with no other effect.

##### 2.4.7.2.3 SDD270 {#SDD270 }

```bool setMode()``` **shall** return ```true``` with no other effect.

##### 2.4.7.2.4 SDD271 {#SDD271 }

```bool subscribe()``` **shall** return ```true``` with no other effect.

#### 2.4.7.3 Constructor {#SDD272 }

##### 2.4.7.3.1 ```AnalogRead(rapidjson::Document d)``` {#SDD273 }

###### 2.4.7.3.1.1 SDD274 {#SDD274 }

This constructor **shall** confirm that ```d``` conforms to schema/analogread_schema.json. If it does not, this constructor **shall** throw a ```std::invalid_argument``` exception.

###### 2.4.7.3.1.2 SDD275 {#SDD275 }

This constructor **shall** initialize all data members as described.

### 2.4.8 SDD085 {#SDD085 }

```RestItem::AnalogWrite``` class

#### 2.4.8.1 Private Members {#SDD276 }

##### 2.4.8.1.1 SDD277 {#SDD277 }

```int pin``` **shall** store the value in the ```pin``` element of the configuration JSON.

##### 2.4.8.1.2 SDD278 {#SDD278 }

```std::string variableName``` **shall** store the value in the ```variable``` element of the configuration JSON.

##### 2.4.8.1.3 SDD279 {#SDD279 }

```int lastValue``` **shall** store the last value successfully written to the aREST target.

##### 2.4.8.1.4 SDD280 {#SDD280 }

```double gain``` **shall** store the value of the ```gain``` element in the configuration JSON, if present. If it is not present, this **shall** default to 1.0f.

##### 2.4.8.1.5 SDD281 {#SDD281 }

```int offset``` **shall** store the value of the ```offset``` element in the configuration JSON, if present. If it is not present, this **shall** default to 0.

##### 2.4.8.1.6 SDD282 {#SDD282 }

```int max``` **shall** store the value of the ```maxValue``` element in the configuration JSON, if present. If it not present, this **shall** default to 255.

##### 2.4.8.1.7 SDD283 {#SDD283 }

```int min``` *shall** store the value of the ```minValue``` element in the configuration JSON, if present. If it not present, this **shall** default to 0.

#### 2.4.8.2 Public Methods {#SDD284 }

##### 2.4.8.2.1 SDD285 {#SDD285 }

```bool poll()``` **shall** return ```true``` with no other effect.

##### 2.4.8.2.2 ```bool procMail(CMOOSMsg &msg)``` {#SDD286 }

###### 2.4.8.2.2.1 SDD287 {#SDD287 }

If ```msg.GetKey()``` is not equal to ```variableName```, this method **shall** return ```false```. This comparison **shall** be case-insensitive.

###### 2.4.8.2.2.2 SDD288 {#SDD288 }

If it is equal, then this method **shall** determine whether the message type is BINARY. If it is anything else, it **shall** return ```false```.

###### 2.4.8.2.2.3 SDD289 {#SDD289 }

If the earlier conditions pass, this method shall construct and execute an aREST request of the form ```/analog/<pin>/<value>```, where ```<value>``` is equal to the received value times the ```gain```, added to the ```offset```, and bounded by ```max``` and ```min```.

###### 2.4.8.2.2.4 SDD290 {#SDD290 }

This method **shall** return the result of the aREST request.

##### 2.4.8.2.3 SDD291 {#SDD291 }

```bool setMode()``` **shall** construct and execute an aREST request of the form ```/mode/<pin>/o``` and return ```true``` if successful. Otherwise, it **shall** return false.

##### 2.4.8.2.4 SDD292 {#SDD292 }

```bool subscribe()``` **shall** subscribe to the variable named by ```variableName```.

##### 2.4.8.2.5 SDD293 {#SDD293 }

```bool setMode()``` **shall** return ```true``` with no other effect.

##### 2.4.8.2.6 SDD294 {#SDD294 }

```bool subscribe()``` **shall** subscribe to the variable named by ```variableName```.

#### 2.4.8.3 Constructor {#SDD295 }

##### 2.4.8.3.1 ```AnalogWrite(rapidjson::Document d)``` {#SDD296 }

###### 2.4.8.3.1.1 SDD297 {#SDD297 }

This constructor **shall** confirm that ```d``` conforms to schema/analogwrite_schema.json. If it does not, this constructor **shall** throw a ```std::invalid_argument``` exception.

###### 2.4.8.3.1.2 SDD298 {#SDD298 }

This constructor **shall** initialize all data members as described.

### 2.4.9 SDD086 {#SDD086 }

```RestItem::Variable``` class

#### 2.4.9.1 Private Members {#SDD299 }

##### 2.4.9.1.1 ```long pollCount``` {#SDD300 }

###### 2.4.9.1.1.1 SDD301 {#SDD301 }

Initialized to zero

###### 2.4.9.1.1.2 SDD302 {#SDD302 }

Incremented each time the ```poll()``` method is called.

##### 2.4.9.1.2 ```long lastPoll``` {#SDD303 }

###### 2.4.9.1.2.1 SDD304 {#SDD304 }

Initialized to zero

###### 2.4.9.1.2.2 SDD305 {#SDD305 }

Set equal to ```pollCount``` every time ```poll()``` successfully reads the target pin.

##### 2.4.9.1.3 SDD306 {#SDD306 }

```std::string name``` **shall** store the value of the ```name``` element in the configuration JSON. This **shall** be used as the name of the variable on the aREST target.

##### 2.4.9.1.4 SDD307 {#SDD307 }

```std::string variableName``` **shall** store the value in the ```variable``` element of the configuration JSON.

##### 2.4.9.1.5 SDD308 {#SDD308 }

```std::string variableType``` **shall** store the value in the ```variableType``` element of the configuration JSON.

#### 2.4.9.2 Public Methods {#SDD309 }

##### 2.4.9.2.1 ```bool poll()``` {#SDD310 }

###### 2.4.9.2.1.1 SDD311 {#SDD311 }

This method **shall** first calculate the number of times it has been called since the last time it made a successful aREST request. If this value is less than ```Configuration::variablePollPeriod```, this method **shall** increment ```pollCount``` and return ```true```.

###### 2.4.9.2.1.2 SDD312 {#SDD312 }

This method **shall** construct and execute an aREST request of the form ```/<name>```.

###### 2.4.9.2.1.3 SDD313 {#SDD313 }

If the aREST request fails, return ```false```.

###### 2.4.9.2.1.4 SDD314 {#SDD314 }

If the value of ```variableType``` is STRING, the response in the JSON **must** be a string value. If it is not, this method **shall** return ```false```.

###### 2.4.9.2.1.5 SDD315 {#SDD315 }

If the value of ```variableType``` is DOUBLE, the response in the JSON **must** be a numerical value. If it is not, this method **shall** return ```false```.

###### 2.4.9.2.1.6 SDD316 {#SDD316 }

If either of the two preceeding conditions are true, this method **shall** publish the returned value to ```variableName```, increment ```pollCount```, set ```lastPoll``` equal to ```pollCount```, and return ```true```. Otherwise, it **shall** return ```false```.

##### 2.4.9.2.2 SDD317 {#SDD317 }

```bool procMail(CMOOSMsg &msg)``` **shall** return ```false``` with no other effect.

##### 2.4.9.2.3 SDD318 {#SDD318 }

```bool setMode()``` **shall** return ```true``` with no other effect.

##### 2.4.9.2.4 SDD319 {#SDD319 }

```bool subscribe()``` **shall** return ```true``` with no other effect.

#### 2.4.9.3 Constructor {#SDD320 }

##### 2.4.9.3.1 ```Variable(rapidjson::Document d)``` {#SDD321 }

###### 2.4.9.3.1.1 SDD322 {#SDD322 }

This constructor **shall** confirm that ```d``` conforms to schema/variable_schema.json. If it does not, this constructor **shall** throw a ```std::invalid_argument``` exception.

###### 2.4.9.3.1.2 SDD323 {#SDD323 }

This constructor **shall** initialize all data members as described.

### 2.4.10 SDD087 {#SDD087 }

```RestItem::Function``` class

#### 2.4.10.1 Private Members {#SDD324 }

##### 2.4.10.1.1 ```long pollCount``` {#SDD325 }

###### 2.4.10.1.1.1 SDD326 {#SDD326 }

Initialized to zero

###### 2.4.10.1.1.2 SDD327 {#SDD327 }

Incremented each time the ```poll()``` method is called.

##### 2.4.10.1.2 ```long lastPoll``` {#SDD328 }

###### 2.4.10.1.2.1 SDD329 {#SDD329 }

Initialized to zero

###### 2.4.10.1.2.2 SDD330 {#SDD330 }

Set equal to ```pollCount``` every time ```poll()``` successfully reads the target pin.

##### 2.4.10.1.3 SDD331 {#SDD331 }

```std::string name``` **shall** store the value of the ```name``` element in the configuration JSON. This **shall** be used as the name of the function on the aREST target.

##### 2.4.10.1.4 SDD332 {#SDD332 }

```std::string returnName``` **shall** store the value in the ```returnVariable``` element of the configuration JSON.

##### 2.4.10.1.5 SDD333 {#SDD333 }

```std::string returnType``` **shall** store the value in the ```returnType``` element of the configuration JSON.

##### 2.4.10.1.6 SDD334 {#SDD334 }

```std::list<RestItem::FunctionParameter*> parameters``` **shall** store the listed parameters of the function.

#### 2.4.10.2 Public Methods {#SDD335 }

##### 2.4.10.2.1 ```bool poll()``` {#SDD336 }

###### 2.4.10.2.1.1 SDD337 {#SDD337 }

This method **shall** first calculate the number of times it has been called since the last time it made a successful aREST request. If this value is less than ```Configuration::variablePollPeriod```, this method **shall** increment ```pollCount``` and return ```true```.

###### 2.4.10.2.1.2 SDD338 {#SDD338 }

This method **shall** construct and execute an aREST request of the form ```/<function>?params=<parameters>```, where the ```<parameters>``` item is created by concatenating the result of calling ```geturlencoded``` against each element of the ```parameters``` list.

###### 2.4.10.2.1.3 SDD339 {#SDD339 }

If the aREST request fails, return ```false```.

###### 2.4.10.2.1.4 SDD340 {#SDD340 }

If the value of ```variableType``` is STRING, the response in the JSON **must** be a string value. If it is not, this method **shall** return ```false```.

###### 2.4.10.2.1.5 SDD341 {#SDD341 }

If the value of ```variableType``` is DOUBLE, the response in the JSON **must** be a numerical value. If it is not, this method **shall** return ```false```.

###### 2.4.10.2.1.6 SDD342 {#SDD342 }

If the value of ```variableType``` is BINARY, the response in the JSON **must** be a boolean value. If it is not, this method **shall** return ```false```.

###### 2.4.10.2.1.7 SDD343 {#SDD343 }

If either of the two preceeding conditions are true, this method **shall** publish the returned value to ```variableName```, increment ```pollCount```, set ```lastPoll``` equal to ```pollCount```, and return ```true```. Otherwise, it **shall** return ```false```.

##### 2.4.10.2.2 ```bool procMail(CMOOSMsg &msg)``` {#SDD344 }

###### 2.4.10.2.2.1 SDD345 {#SDD345 }

If ```parameters``` is empty, this method **shall** return ```false```.

###### 2.4.10.2.2.2 SDD346 {#SDD346 }

This method **shall** iterate through the ```parameters``` list, calling the ```procMail()``` method of each member.

###### 2.4.10.2.2.3 SDD347 {#SDD347 }

If any of these calls return ```true```, this method **shall** set ```lastPoll``` equal to 0 and return ```true```.

###### 2.4.10.2.2.4 SDD348 {#SDD348 }

If none return ```true```, this method **shall** return ```false```.

##### 2.4.10.2.3 SDD349 {#SDD349 }

```bool setMode()``` **shall** return ```true``` with no other effect.

##### 2.4.10.2.4 ```bool subscribe()``` {#SDD350 }

###### 2.4.10.2.4.1 SDD351 {#SDD351 }

The method **shall** call the ```subscribe()``` method on each member of the ```parameters``` list.

###### 2.4.10.2.4.2 SDD352 {#SDD352 }

This method **shall** return the logical and of the results of all the aforementioned calls, and ```true``` if the ```parameters``` list is empty.

#### 2.4.10.3 Constructor {#SDD353 }

##### 2.4.10.3.1 ```Function(rapidjson::Document d)``` {#SDD354 }

###### 2.4.10.3.1.1 SDD355 {#SDD355 }

This constructor **shall** confirm that ```d``` conforms to schema/function_schema.json. If it does not, this constructor **shall** throw a ```std::invalid_argument``` exception.

###### 2.4.10.3.1.2 SDD356 {#SDD356 }

This constructor **shall** initialize all data members as described.

###### 2.4.10.3.1.3 SDD357 {#SDD357 }

This constructor shall call ```RestItem::FunctionParameter::functionParameterFactory()``` against each element of the ```parameters``` JSON array, if present, and store the results in the ```parameters``` member.

## 2.5 ```Configuration``` class {#SDD088 }

### 2.5.1 SDD089 {#SDD089 }

This class **shall** be a singleton.

### 2.5.2 Members {#SDD090 }

#### 2.5.2.1 ```interface``` {#SDD092 }

##### 2.5.2.1.1 SDD093 {#SDD093 }

Type ```RestInterface*```

##### 2.5.2.1.2 SDD094 {#SDD094 }

Default ```nullptr```

##### 2.5.2.1.3 SDD095 {#SDD095 }

Contains a pointer to the interface used by this configuration

##### 2.5.2.1.4 SDD096 {#SDD096 }

This must be a valid pointer in order for the configuration to be valid.

#### 2.5.2.2 ```digitalPollPeriod``` {#SDD097 }

##### 2.5.2.2.1 SDD098 {#SDD098 }

Type ```int```

##### 2.5.2.2.2 SDD099 {#SDD099 }

Default 0

##### 2.5.2.2.3 SDD100 {#SDD100 }

Populated from the configuration parameter of the same name.

#### 2.5.2.3 ```analogPollPeriod``` {#SDD101 }

##### 2.5.2.3.1 SDD102 {#SDD102 }

Type ```int```

##### 2.5.2.3.2 SDD103 {#SDD103 }

Default 0

##### 2.5.2.3.3 SDD104 {#SDD104 }

Populated from the configuration parameter of the same name.

#### 2.5.2.4 ```variablePollPeriod``` {#SDD105 }

##### 2.5.2.4.1 SDD106 {#SDD106 }

Type ```int```

##### 2.5.2.4.2 SDD107 {#SDD107 }

Default 0

##### 2.5.2.4.3 SDD108 {#SDD108 }

Populated from the configuration parameter of the same name.

#### 2.5.2.5 ```functionPollPeriod``` {#SDD109 }

##### 2.5.2.5.1 SDD110 {#SDD110 }

Type ```int```

##### 2.5.2.5.2 SDD111 {#SDD111 }

Default 0

##### 2.5.2.5.3 SDD112 {#SDD112 }

Populated from the configuration parameter of the same name.

#### 2.5.2.6 ```pollList``` {#SDD113 }

##### 2.5.2.6.1 SDD114 {#SDD114 }

Type ```std::list<RestItem::RestItem*>```

##### 2.5.2.6.2 SDD115 {#SDD115 }

Default empty list

#### 2.5.2.7 ```interfaceSchema``` {#SDD116 }

##### 2.5.2.7.1 SDD117 {#SDD117 }

Type ```rapidjson::Document```

##### 2.5.2.7.2 SDD118 {#SDD118 }

Stores schema for the ```interface``` type

#### 2.5.2.8 ```digitalWriteSchema``` {#SDD119 }

##### 2.5.2.8.1 SDD120 {#SDD120 }

Type ```rapidjson::Document```

##### 2.5.2.8.2 SDD121 {#SDD121 }

Stores schema for the ```RestItem::DigitalWrite``` type

#### 2.5.2.9 ```digitalReadSchema``` {#SDD122 }

##### 2.5.2.9.1 SDD123 {#SDD123 }

Type ```rapidjson::Document```

##### 2.5.2.9.2 SDD124 {#SDD124 }

Stores schema for the ```RestItem::DigitalRead``` type

#### 2.5.2.10 ```analogWriteSchema``` {#SDD125 }

##### 2.5.2.10.1 SDD126 {#SDD126 }

Type ```rapidjson::Document```

##### 2.5.2.10.2 SDD127 {#SDD127 }

Stores schema for the ```RestItem::AnalogWrite``` type

#### 2.5.2.11 ```analogReadSchema``` {#SDD128 }

##### 2.5.2.11.1 SDD129 {#SDD129 }

Type ```rapidjson::Document```

##### 2.5.2.11.2 SDD130 {#SDD130 }

Stores schema for the ```RestItem::AnalogRead``` type

#### 2.5.2.12 ```variableSchema``` {#SDD131 }

##### 2.5.2.12.1 SDD132 {#SDD132 }

Type ```rapidjson::Document```

##### 2.5.2.12.2 SDD133 {#SDD133 }

Stores schema for the ```RestItem::Variable``` type

#### 2.5.2.13 ```functionSchema``` {#SDD134 }

##### 2.5.2.13.1 SDD135 {#SDD135 }

Type ```rapidjson::Document```

##### 2.5.2.13.2 SDD136 {#SDD136 }

Stores schema for the ```RestItem::Function``` type

### 2.5.3 Public Methods {#SDD137 }

#### 2.5.3.1 SDD138 {#SDD138 }

```instance()``` returns a pointer to itself.

#### 2.5.3.2 SDD139 {#SDD139 }

This class **shall** define a const reader method for each data member which returns a const pointer.

#### 2.5.3.3 operator bool() {#SDD140 }

##### 2.5.3.3.1 SDD141 {#SDD141 }

Returns true if and only if ```interface``` contains a pointer to a valid ```RestInterface``` object.

#### 2.5.3.4 ```bool populate(std::string param, std::string value)``` {#SDD142 }

##### 2.5.3.4.1 SDD143 {#SDD143 }

Takes a ```param``` and a ```value``` from the configuration file.

##### 2.5.3.4.2 SDD144 {#SDD144 }

If ```param``` names an ```int``` member and ```value``` contains an int value, this method **shall** populate that member.

##### 2.5.3.4.3 SDD145 {#SDD145 }

If the ```param``` is ```interface```, the ```value``` **shall** be parsed with rapidjson::Parse() and the resulting document **shall** be passed to ```RestInterface::factory()```. If the resulting pointer is not null, it shall be assigned to the ```interface``` member.

##### 2.5.3.4.4 SDD146 {#SDD146 }

If the ```param``` is ```confFile```, it **shall** pass ```value``` to ```confFileReader``` and returns the result.

##### 2.5.3.4.5 SDD147 {#SDD147 }

If the ```value``` otherwise contains JSON, it **shall** pass it to ```jsonDispatch``` and return the result.

##### 2.5.3.4.6 SDD148 {#SDD148 }

Returns ```true``` if and only if ```param``` names a valid configuration parameter and ```value``` is a valid value for that parameter. Otherwise, it **shall** return ```false```.

#### 2.5.3.5 ```void poll()``` {#SDD149 }

##### 2.5.3.5.1 SDD150 {#SDD150 }

This method **shall** iterate through ```pollList``` calling the ```poll()``` method on each item.

##### 2.5.3.5.2 SDD358 {#SDD358 }

If any of these calls return false, this method **shall** call ```reportRunWarning()``` with information about the failed call.

#### 2.5.3.6 ```bool procMail(CMOOSMsg &msg)``` {#SDD151 }

##### 2.5.3.6.1 SDD152 {#SDD152 }

This method **shall** iterate through ```pollList``` calling the ```procMail()``` method on each item. It **shall** return ```true``` if any of these calls return ```true```.

#### 2.5.3.7 ```bool subscribe()``` {#SDD359 }

##### 2.5.3.7.1 SDD360 {#SDD360 }

This method **shall** call the ```subscribe()``` method of each member of ```pollList```.

##### 2.5.3.7.2 SDD361 {#SDD361 }

This method **shall** return the logical and of the return values of the ```subscribe()``` methods of each member.

#### 2.5.3.8 ```bool setMode()``` {#SDD362 }

##### 2.5.3.8.1 SDD363 {#SDD363 }

This method **shall** call the ```setMode()``` method of each member of ```pollList```.

##### 2.5.3.8.2 SDD364 {#SDD364 }

This method **shall** return the logical and of the return values of the ```setMode()``` methods of each member.

#### 2.5.3.9 ```void dedupe()``` {#SDD365 }

##### 2.5.3.9.1 SDD366 {#SDD366 }

This method **shall** remove duplicate members of ```pollList```

#### 2.5.3.10 ```operator bool()``` {#SDD153 }

##### 2.5.3.10.1 SDD154 {#SDD154 }

Returns ```true``` if and only if ```interface``` has been properly configured.

### 2.5.4 Private Methods {#SDD155 }

#### 2.5.4.1 ```bool confFileReader(std::string filename)``` {#SDD156 }

##### 2.5.4.1.1 SDD157 {#SDD157 }

Takes the name of a configuration file.

##### 2.5.4.1.2 SDD158 {#SDD158 }

This **shall** return ```false``` if the configuration file does not contain a single JSON array.

##### 2.5.4.1.3 SDD159 {#SDD159 }

Calls ```jsonDispatch``` against each member of the JSON array and returns ```true``` if and only if all calls return ```true```.

#### 2.5.4.2 ```bool jsonDispatch(rapidjson::Document d)``` {#SDD160 }

##### 2.5.4.2.1 SDD161 {#SDD161 }

Takes a JSON object that conforms to one of the stored schemas. If it does not conform, this method **shall** return false.

##### 2.5.4.2.2 SDD162 {#SDD162 }

If ```d``` conforms to the ```interfaceSchema```, the ```interface``` method **shall** be populated by calling ```d``` to ```RestInterface::factory()``` and assigning the result to ```interface```.

##### 2.5.4.2.3 SDD163 {#SDD163 }

Otherwise, pass ```d``` to ```RestItem::factory()```. If the result is a valid pointer, it **shall** be added to the end of ```pollList``` and this method **shall** return ```true```.

##### 2.5.4.2.4 SDD164 {#SDD164 }

If none of these conditions are true, this method **shall** return ```false```.

### 2.5.5 Constructor {#SDD165 }

#### 2.5.5.1 ```Configuration()``` {#SDD166 }

##### 2.5.5.1.1 SDD167 {#SDD167 }

**Shall** initialize all members to their default values.

##### 2.5.5.1.2 SDD168 {#SDD168 }

Populates schemas as described in the ```*Schema``` members.

# 3.0 Startup Behavior {#SDD010 }

## 3.1 SDD367 {#SDD367 }

The ```OnStartUp()``` method **shall** iterate through the MOOS configuration file and pass each line of the configuration file to the ```Configuration::populate()``` method in turn.

## 3.2 SDD368 {#SDD368 }

If ```Configuration::populate()``` returns ```false```, the offending line **shall** be passed to ```reportUnhandledConfigWarning()```.

## 3.3 SDD369 {#SDD369 }

After all configuration lines have been processed, this software **shall** call ```Configuration::dedupe()```.

## 3.4 SDD370 {#SDD370 }

At the end of the configuration process, ```OnStartUp()``` shall call the ```Configuration``` object's bool operator. If it returns ```false```, this program **shall** log an error and exit.

# 4.0 SDD012 {#SDD012 }

MOOS Community Connection Behavior

## 4.1 SDD371 {#SDD371 }

The ```OnConnectToServer()``` method **shall** call ```Configuration::subscribe()``` and log the result.

## 4.2 SDD372 {#SDD372 }

The ```OnConnectToServer()``` method **shall** call ```Configuration::setMode()``` and log the result.

# 5.0 SDD013 {#SDD013 }

New Mail Behavior

## 5.1 SDD373 {#SDD373 }

The ```OnStartUp()``` method **shall** iterate through the incoming mail list and pass each element of the list to the ```Configuration::procMail()``` method in turn.

## 5.2 SDD374 {#SDD374 }

If ```Configuration::procMail()``` returns ```false```, the offending line **shall** be passed to ```reportRunWarning()``` with the value ```"Unhandled Mail: " + key```.

# 6.0 SDD014 {#SDD014 }

App Tick Behavior

## 6.1 SDD375 {#SDD375 }

The ```Iterate()``` method **shall** call ```Configuration::poll()```.

# 7.0 Appcast Reporting Behavior {#SDD376 }

## 7.1 SDD377 {#SDD377 }

Using the reader, ```buildReport()``` **shall** iterate through the contents of ```Configuration::pollList``` and add the results of ```reportHeader()``` and ```reportLine()``` from each member.

