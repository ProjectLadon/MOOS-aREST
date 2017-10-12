# MOOS-aREST
A connector between MOOS and aREST running on Arduino satellite nodes. It provides a direct linkage between aREST endpoints and MOOS variables.

# Configuration Options
* interface -- defines the aRest interface to be used, according to the following schema:
```
{
	"$schema": "http://json-schema.org/schema#",
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
						"$comment":"Pattern needs to be filled in so it only permits sane values"
						"$comment":"Defaults to 8N1"
					}
				},
				"required":["interfaceType", "interface"]
				"$comment": "This is for a serial interface"
			},
			{
				"type":"object",
				"properties": {
					"interfaceType": "network",
					"url": {"type":"string", "format": "uri"},
				},
				"required":["interfaceType", "url"]
				"$comment": "This is for a network interface"
			}
		]
	}
}
```
* digitalRead -- read a given digital pin and publish the result on MOOSDB as a BINARY, as defined by the following JSON schema:
```
{
	"$schema": "http://json-schema.org/schema#",
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
* digitalWrite -- write from a given MOOSDB BINARY variable to a given digital pin, as defined by the following JSON schema:
```
{
	"$schema": "http://json-schema.org/schema#",
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
* analogRead -- read a given analog pin and publish the result on MOOSDB as a DOUBLE, as defined by the following JSON schema:
```
{
	"$schema": "http://json-schema.org/schema#",
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
* analogWrite -- write from a given MOOSDB DOUBLE variable to a given analog pin, as defined by the following JSON schema:
```
{
	"$schema": "http://json-schema.org/schema#",
	"type": "object",
	"properties":{
		"function":"analogWrite",
		"pin":{"type":"number"},
		"variable":{
			"type":"string",
			"$comment": "This must be an existing DOUBLE variable"
		}
		"offset":{"type":"integer"},
		"gain":{"type":"number"}
		"maxValue":{"type":"integer"},
		"minValue":{"type":"integer"}
	},
	"required":["function","pin","variable"]
}
```
* variable -- read a given analog pin and publish the result on MOOSDB as a DOUBLE, as defined by the following JSON schema:
```
{
	"$schema": "http://json-schema.org/schema#",
	"type": "object",
	"properties":{
		"function":"Variable",
		"pin":{"type":"number"},
		"variableType":{"type":"string","enum":["STRING","DOUBLE"]}
		"variable":{
			"type":"string",
			"$comment": "This must be a free variable name; it will be published as a DOUBLE"
		}
	},
	"required":["function","pin","variable","variableType"]
}
```
* function
* confFile -- path to a configuration file with any combination of other objects. 
* digitalPollPeriod -- polling period for digitalRead items, in microseconds. 
* analogPollPeriod -- polling period for analogRead items, in microseconds. 
* variablePollPeriod -- polling period for variable items, in microseconds.
