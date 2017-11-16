# 1.0 MOOS aREST interface module {#SRD001 }

This module provides a mechanism for a MOOS community to use an Arduino-compatible board running an aREST sketch as a physical interface device. It provides interface routines for all of the major aREST calls.

This library uses libcurl as the communication mechanism.

Implementation of a serial interface is planned for the future but has not been implemented for this iteration.

## 1.1 SRD002 {#SRD002 }

Software Structure & Dependencies

### 1.1.1 SRD035 {#SRD035 }

This module **shall** be implemented as a MOOS Appcasting App as describe in the (http://oceanai.mit.edu/moos-ivp/pmwiki/pmwiki.php)[MOOS-IvP wiki].

### 1.1.2 SRD036 {#SRD036 }

This module **shall** use (http://www.rapidjson.org)[rapidjson] as the JSON parse/unparse library.

### 1.1.3 SRD037 {#SRD037 }

This module **shall** use libcurl as the HTTP client.

## 1.2 MOOS Startup Configuration Options {#SRD049 }

### 1.2.1 SRD003 {#SRD003 }

The aRESt interface **shall** be defined by a JSON document that is compliant with the following JSON schmema. This JSON document **shall** be presented either as the value of the key ```interface``` in the MOOS configuration or as an element in the JSON array contained in the file named by the ```confFile``` key. If both are present, the document in the MOOS configuration **shall** take precendence.

Note that this JSON schema is provided here for reference only -- the authoritative version **shall** be stored in MOOS-paREST/schema/interface_schema.json.

```
{
  "$schema": "http://json-schema.org/schema#",
  "id": "AREST_interface",
  "type": "object",
  "properties":{
    "oneOf": [
      {
        "type":"object",
        "properties": {
          "interfaceType": "serial",
          "interface": {
            "type":"string",
            "pattern": "",
            "$comment": "pattern needs to be filled in so it only accepts device names"
          },
          "speed": {"type":"integer", "$comment":"defaults to 115200"},
          "serialFormat": {
            "type":"string",
            "pattern": "",
            "$comment":"Pattern needs to be filled in so it only permits sane values",
            "$comment":"Defaults to 8N1"
          }
        },
        "required":["interfaceType", "interface"],
        "$comment": "This is for a serial interface"
      },
      {
        "type":"object",
        "properties": {
          "interfaceType": "network",
          "url": {"type":"string", "format": "uri"},
        },
        "required":["interfaceType", "url"],
        "$comment": "This is for a network interface"
      }
    ]
  }
}
```

#### 1.2.1.1 SRD004 {#SRD004 }

* The ```interfaceType``` element **must** be equal to either ```serial``` or ```network```. All other values **shall** render this object invalid.

#### 1.2.1.2 SRD005 {#SRD005 }

If the value of ```interfaceType``` is ```network```, the only other key **must** be ```url```, which **shall** contain the root URI of the target aREST interface. The document **must** not contain additional keys.

#### 1.2.1.3 SRD006 {#SRD006 }

If the value is ```serial```, the document **must** contain an ```interface``` key which **must** contain the name of the serial serial interface attached to the aREST target. A ```serial``` document **may** also contain ```speed``` and ```serialFormat``` keys.

#### 1.2.1.4 SRD038 {#SRD038 }

The ```speed``` key, if present, **must** be interpreted as a serial speed in bps. If this key is not present, this module **must** default to 115200 (bps).

#### 1.2.1.5 SRD039 {#SRD039 }

The ```serialFormat``` key, if present, **must** be interepreted as the same format as 8N1 (eight data bits, no parity, 1 stop bit). This **shall** default to 8N1 if not present. Only 8N1 must be supported.

### 1.2.2 SRD050 {#SRD050 }

The ```digitalPollPeriod``` **shall** specify the interval in ticks between consecutives reads of the digital inputs. If the interval is 0, the digital inputs **shall** be polled on every tick.

### 1.2.3 SRD051 {#SRD051 }

The ```analogPollPeriod``` **shall** specify the interval in ticks between consecutives reads of the analog inputs. If the interval is 0, the analog inputs **shall** be read on every tick.

### 1.2.4 SRD052 {#SRD052 }

The ```variablePollPeriod``` **shall** specify the interval in ticks between consecutives reads of the variables. If the interval is 0, the variables **shall** be polled on every tick.

#### 1.2.4.1 SRD053 {#SRD053 }

If ```variablePollPeriod``` is set and variables are returned that are not named by corresponding ```variable``` declarations, those variables **shall** be published under names formed by prepending the name of the calling process and an underscore.

For example, if a variable named ```voltage``` were collected and the name of this process were ```aREST```, the resulting variable would be named ```aREST_voltage```.

> `requirements/SRD012.yml`

#### 1.2.4.2 SRD054 {#SRD054 }

The type of the published variable **shall** be deduced from the JSON type received from the aREST input. Numbers **shall** be published as DOUBLE, booleans **shall** be published as BINARY, and all others **shall** be published as STRING.

### 1.2.5 SRD070 {#SRD070 }

The ```functionPollPeriod``` **shall** specify the interval, in ticks, between successive function calls. If the interval is 0, the function(s) shall be polled on every tick. Note that this applies both to functions that have inputs and those that do not.

### 1.2.6 SRD055 {#SRD055 }

If ```confFile``` is present and points to a valid JSON document, it **shall** be interpreted in the manner define in the referenced section.

> `requirements/SRD007.yml`

## 1.3 Configuration of aREST items {#SRD007 }

The aREST items touched by this are each configured with a JSON document, as detailed in this section.

### 1.3.1 SRD008 {#SRD008 }

Each ```digitalRead``` item **shall** be defined by a JSON document that is compliant with the following JSON schema. This JSON document **shall** be presented either as the value of a ```digitalRead``` key or as an element in the JSON array contained in the file named by the ```confFile``` key.

Note that this JSON schema is provided here for reference only -- the authoritative version **shall** be stored in MOOS-paREST/schema/digitalRead_schema.json.

```
{
  "$schema": "http://json-schema.org/schema#",
  "id": "AREST_digitalRead",
  "type": "object",
  "properties":{
    "function":"digitalRead",
    "pin":{"type":"number"},
    "variable":{
      "type":"string",
      "$comment": "This must be a free variable name; it will be published as a BINARY"
    }
  },
  "required":["function","pin","variable"]
}
```

#### 1.3.1.1 SRD009 {#SRD009 }

The ```function``` key **shall** have the value of ```digitalRead```. Any other value shall be invalid for this item, but **may** be valid for others.

#### 1.3.1.2 SRD033 {#SRD033 }

The ```pin``` key **shall** name an integer value corresponding to the pin to be read.

#### 1.3.1.3 SRD034 {#SRD034 }

The ```variable``` key **shall** name the variable to publish the retrieved value under. It **shall** be published as a BINARY.

### 1.3.2 SRD010 {#SRD010 }

Each ```digitalWrite``` item **shall** be defined by a JSON document that is compliant with the following JSON schema. This JSON document **shall** be presented either as the value of a ```digitalWrite``` key or as an element in the JSON array contained in the file named by the ```confFile``` key.

Note that this JSON schema is provided here for reference only -- the authoritative version **shall** be stored in MOOS-paREST/schema/digitalWrite_schema.json.

```
{
  "$schema": "http://json-schema.org/schema#",
  "id": "AREST_digitalWrite",
  "type": "object",
  "properties":{
    "function":"digitalWrite",
    "pin":{"type":"number"},
    "variable":{
      "type":"string",
      "$comment": "This must be an existing BINARY variable"
    }
  },
  "required":["function","pin","variable"]
}
```

#### 1.3.2.1 SRD011 {#SRD011 }

The ```function``` key **shall** have the value of ```digitalWrite```. Any other value shall be invalid for this item, but **may** be valid for others.

#### 1.3.2.2 SRD031 {#SRD031 }

The ```pin``` key **shall** name an integer value corresponding to the pin to be written.

#### 1.3.2.3 SRD032 {#SRD032 }

The ```variable``` key **shall** name the variable to subscribe to in order to drive this variable. This variable **must** be a BINARY; if it has some other type, the resulting behavior is undefined.

### 1.3.3 SRD012 {#SRD012 }

Each ```analogRead``` item **shall** be defined by a JSON document that is compliant with the following JSON schema. This JSON document **shall** be presented either as the value of a ```analogRead``` key or as an element in the JSON array contained in the file named by the ```confFile``` key.

Note that this JSON schema is provided here for reference only -- the authoritative version **shall** be stored in MOOS-paREST/schema/analogRead_schema.json.

```
{
  "$schema": "http://json-schema.org/schema#",
  "id": "AREST_analogRead",
  "type": "object",
  "properties":{
    "function":"analogRead",
    "pin":{"type":"number"},
    "variable":{
      "type":"string",
      "$comment": "This must be a free variable name; it will be published as a DOUBLE"
    },
    "offset":{"type":"integer"},
    "gain":{"type":"number"}
  },
  "required":["function","pin","variable"]
}
```

#### 1.3.3.1 SRD013 {#SRD013 }

The ```function``` key **shall** have the value of ```analogRead```. Any other value shall be invalid for this item, but **may** be valid for others.

#### 1.3.3.2 SRD027 {#SRD027 }

The ```pin``` key **shall** name an integer value corresponding to the analog pin to be read.

#### 1.3.3.3 SRD028 {#SRD028 }

The ```variable``` key **shall** name the variable to publish the retrieved value under. It **shall** be published as a DOUBLE.

#### 1.3.3.4 SRD029 {#SRD029 }

The ```offset``` value **shall** be added to the received value prior to publication. If the ```offset``` key is absent, it **shall** default to 0.

#### 1.3.3.5 SRD030 {#SRD030 }

The received value shall be multiplied by the ```gain``` value after the ```offset``` value has been added. If the ```gain``` value is not present it **shall** default to 1.

### 1.3.4 SRD014 {#SRD014 }

Each ```analogWrite``` item **shall** be defined by a JSON document that is compliant with the following JSON schema. This JSON document **shall** be presented either as the value of a ```analogWrite``` key or as an element in the JSON array contained in the file named by the ```confFile``` key.

Note that this JSON schema is provided here for reference only -- the authoritative version **shall** be stored in MOOS-paREST/schema/analogWrite_schema.json.

```
{
  "$schema": "http://json-schema.org/schema#",
  "id": "AREST_analogWrite",
  "type": "object",
  "properties":{
    "function":"analogWrite",
    "pin":{"type":"number"},
    "variable":{
      "type":"string",
      "$comment": "This must be an existing DOUBLE variable"
    },
    "offset":{"type":"integer"},
    "gain":{"type":"number"},
    "maxValue":{"type":"integer"},
    "minValue":{"type":"integer"}
  },
  "required":["function","pin","variable"]
}
```

#### 1.3.4.1 SRD015 {#SRD015 }

The ```function``` key **shall** have the value of ```analogWrite```. Any other value shall be invalid for this item, but **may** be valid for others.

#### 1.3.4.2 SRD021 {#SRD021 }

The ```pin``` key **shall** name an integer value corresponding to the analog pin to be written.

#### 1.3.4.3 SRD022 {#SRD022 }

This module **shall** subscribe to the DOUBLE variable named by ```variable``` key.

#### 1.3.4.4 SRD023 {#SRD023 }

The ```offset``` value **shall** be added to the subscribed value prior to transmission. If the ```offset``` key is absent, it **shall** default to 0.

#### 1.3.4.5 SRD024 {#SRD024 }

The subscribed value shall be multiplied by the ```gain``` value after the ```offset``` value has been added. If the ```gain``` value is not present it **shall** default to 1.

#### 1.3.4.6 SRD025 {#SRD025 }

If ```maxValue``` exists, it **shall** be the largest value that will be written to the target pin.

#### 1.3.4.7 SRD026 {#SRD026 }

If ```minValue``` exists, it **shall** be the smallest value that will be written to the target pin.

### 1.3.5 SRD016 {#SRD016 }

Each ```variable``` item **shall** be defined by a JSON document that is compliant with the following JSON schema. This JSON document **shall** be presented either as the value of a ```variable``` key or as an element in the JSON array contained in the file named by the ```confFile``` key.

Note that this JSON schema is provided here for reference only -- the authoritative version **shall** be stored in MOOS-paREST/schema/variable_schema.json.

```
{
  "$schema": "http://json-schema.org/schema#",
  "id": "AREST_variable",
  "type": "object",
  "properties":{
    "function":"variable",
    "name":{"type":"string"},
    "variableType":{"type":"string","enum":["STRING","DOUBLE"]},
    "variable":{
      "type":"string",
      "$comment": "This must be a free variable name; it will be published as a DOUBLE"
    }
  },
  "required":["function","name","variable","variableType"]
}
```

#### 1.3.5.1 SRD017 {#SRD017 }

The ```function``` key **shall** have the value of ```variable```. Any other value shall be invalid for this item, but **may** be valid for others.

#### 1.3.5.2 SRD018 {#SRD018 }

The ```name``` key **shall** be the name of the variable queried from the aREST endpoint.

#### 1.3.5.3 SRD019 {#SRD019 }

The ```variableType``` key **shall** name the type of the variable published to the MOOS community. It **must** have a value of either ```STRING``` or ```DOUBLE```.

#### 1.3.5.4 SRD020 {#SRD020 }

The ```variable``` key **shall** be the name this variable is published under to the MOOS community.

### 1.3.6 SRD040 {#SRD040 }

Each ```function``` item **shall** be defined by a JSON document that is compliant with the following JSON schema. This JSON document **shall** be presented either as the value of a ```function``` key or as an element in the JSON array contained in the file named by the ```confFile``` key.

Note that this JSON schema is provided here for reference only -- the authoritative version **shall** be stored in MOOS-paREST/schema/function_schema.json.

```
{
  "$schema": "http://json-schema.org/schema#",
  "id": "AREST_function",
  "type": "object",
  "properties":{
    "function":"function",
    "name":{"type":"string"},
    "parameters":{
      "type":"array",
      "items":{
        "type": "object",
        "properties": {
          "name": {"type":"string"},
          "inputVariable": {"type":"string"},
          "inputVariableType": {"type": "string", "enum":["DOUBLE", "STRING", "BINARY"]}
        },
        "required":["name", "inputVariable", "inputVariableType"]
      }
    },
    "returnType":{"type":"string","enum":["STRING","DOUBLE","BINARY"]},
    "returnVariable":{
      "type":"string",
      "$comment": "This must be a free variable name; it will be published as specified by returnType"
    }
  },
  "required":["function","name"]
}
```

#### 1.3.6.1 SRD041 {#SRD041 }

The ```function``` key **shall** have the value of ```function```. Any other value shall be invalid for this item, but **may** be valid for others.

#### 1.3.6.2 SRD042 {#SRD042 }

The ```name``` key **shall** be the name of the function called on the aREST endpoint.

#### 1.3.6.3 SRD043 {#SRD043 }

The ```parameters``` key **shall** be optional. If present, it **must** be an array of one or more JSON objects, each of which specifies an input parameter to the called function.

##### 1.3.6.3.1 SRD044 {#SRD044 }

The ```name``` key **shall** be the name of the argument as passed to the aREST endpoint function.

##### 1.3.6.3.2 SRD045 {#SRD045 }

The ```inputVariable``` key **shall** be the name of the MOOS variable whose value is used for this argument.

##### 1.3.6.3.3 SRD046 {#SRD046 }

The ```inputVariableType``` key **shall** name the type of ```inputVariable```. It **must** be a MOOS variable type, i.e. either STRING, DOUBLE, or BINARY.

#### 1.3.6.4 SRD047 {#SRD047 }

The ```returnType``` key **shall** name the type of variable returned by the function. It **must** be a MOOS variable type, i.e. either STRING, DOUBLE, or BINARY.

#### 1.3.6.5 SRD048 {#SRD048 }

This module **shall** publish the value returned by the called function to the MOOS variable named by ```returnVariable```.

# 2.0 MOOS Startup Behavior {#SRD056 }

This section defines how this module functions during the startup period, i.e. that period of time controlled by the OnStartUp() function.

## 2.1 SRD057 {#SRD057 }

Startup configuration options **shall** be processed in the order presented. Items configured by the file pointed at by ```confFile``` shall be processed in array order at the point in the confgiuration where the ```confFile``` value is defined.

## 2.2 SRD058 {#SRD058 }

```digitalRead```, ```digitalWrite```, and ```analogWrite``` **shall** be treated as mutually exclusive with respect to any given pin. If multiple items are defined for the same pin, the last one **must** be implemented.

## 2.3 SRD059 {#SRD059 }

At the end of the startup phase, this module **shall** attempt to configure the mode of all pins that correspond to defined items.

### 2.3.1 SRD060 {#SRD060 }

If this module is unable to configure a pin for any reason, it **shall** attempt to do so on any future calls to that pin until that pin has been successfully configured.

### 2.3.2 SRD061 {#SRD061 }

Any failure to configure a pin **shall** be logged.

# 3.0 MOOS Network Connect Behavior {#SRD077 }

This section defines how the module behaves when it connects to the network, i.e. when it is under control of the OnConnectToServer function.

## 3.1 SRD078 {#SRD078 }

On connection to the server, this module **shall** register for all variables called for in ```digitalWrite```, ```analogWrite```, and ```function``` items.

# 4.0 MOOS Comms Behavior {#SRD062 }

This section defines how the module behaves when it receives new mail, i.e. when it is under control of the OnNewMail() function.

## 4.1 SRD063 {#SRD063 }

On receipt of a new value attached to any ```digitalWrite```,  ```analogWrite```, or ```function``` item, that item **shall** be transmitted to the aREST end point.

## 4.2 SRD064 {#SRD064 }

If a ```function``` item has more than one parameter and not all of them are present, the last received value **shall** be used.

### 4.2.1 SRD065 {#SRD065 }

If no value has been received for a given parameter, then a default value **shall** be transmitted. The default values used **shall** be as follows:
* STRING = ""
* DOUBLE = 0
* BINARY = false

## 4.3 SRD066 {#SRD066 }

Any variable values received on a ```function``` call **shall** be published in accordance with a previously defined ```variable``` item or as described in the referenced requirements. This behavior **shall** be independent of ```variablePollPeriod```.

> `requirements/SRD053.yml`

## 4.4 SRD067 {#SRD067 }

Return values from function calls **shall** be published upon receipt.

## 4.5 SRD068 {#SRD068 }

Any failed calls **shall** be logged.

# 5.0 MOOS Tick Behavior {#SRD069 }

This section defines how the module behaves when under the control of the Iterate() function.

## 5.1 SRD071 {#SRD071 }

If any pins that correspond to items have not had their modes successfully set, this module **shall** attempt to set their modes at the start of each tick.

## 5.2 SRD072 {#SRD072 }

For the purposes of this section, 'polling' **shall** mean making a request to the corresponding aREST input via the configured interface.

## 5.3 SRD073 {#SRD073 }

If ```digitalPollPeriod``` ticks have elapsed since the last poll of the digital inputs, all digital inputs **shall** be polled. This **may**, if supported by aREST, poll all digital inputs with a single request.

## 5.4 SRD074 {#SRD074 }

If ```analogPollPeriod``` ticks have elapsed since the last poll of the analog inputs, all analog inputs **shall** be polled. This **may**, if supported by aREST, poll all analog inputs with a single request.

## 5.5 SRD075 {#SRD075 }

If ```variablePollPeriod``` ticks have elapsed since the last poll of the variables, all variables **shall** be polled. This **may**, if supported by aREST, poll all variables with a single request. It **shall not** be impacted by incidental fetching of the variables, such as by function calls.

### 5.5.1 SRD076 {#SRD076 }

If any variables are received that are not defined by corresponding ```variable``` declarations, those variables **shall** be treated as per the referenced requirement.

> `requirements/SRD053.yml`

