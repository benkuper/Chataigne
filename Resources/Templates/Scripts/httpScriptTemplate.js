

/* ********** HTTP MODULE SPECIFIC SCRIPTING ********************* */
/*

HTTP Mdule has specific methods that can be used to handle receiving and sending data over the connection.
With HTTP modules, there are different ways of sending data.

1) You can do it inline, passing all arguments in the function. 

If you using GET/PUT/PATCH/DELETE, the function looks like this :
local.sendGET(address, [dataType], [extraHeaders], [payload]); //The dataType, extraHeaders and payload arguments are optional.

If you're using POST, the function looks like this :
local.sendPOST(address, argName1, argValue1, argName2, argValue2); //After address, values are taken 2 by 2 to form a name/value pair.

Here are some examples to get you going :

local.sendGET("https://httpbin.org/anything?myValue1=1&myValue2=super", "json", "Content-Type: text/plain", "some payload");
local.sendPUT("https://httpbin.org/anything?myValue1=1&myValue2=super");
local.sendPATCH("https://httpbin.org/anything?myValue1=1&myValue2=super");
local.sendDELETE("https://httpbin.org/anything?myValue1=1&myValue2=super");
local.sendPOST("https://httpbin.org/anything", "myValue1", 1, "myValue2", "super"); //After address, values are taken 2 by 2 to form a name/value pair.


2) Or you can create an object to hold the request parameters

var params = {};
params.dataType = "json";
params.extraHeaders = "Content-Type: application/json";
params.arguments = ["myValue1",1,"myValue2",2];

var payload = {}; //the payload can be either a simple string or an object that will be automatically stringified
payload.super = "cool";
payload.number = 3.2;
params.payload = payload;


local.sendGET("anything", params); //the address field will be appended to the module's base address
local.sendPOST("anything", params);
local.sendPUT("anything", params);
local.sendPATCH("anything", params);
local.sendDELETE("anything", params);

Few notes : 
If you want the payload to be detected as data or json, you need to set "Content-Type: text/plain" or "application/json" in extraHeaders
If you're using POST, you can't set both arguments and payload, they will just be appended to form a monstrous data blob that no machine on earth can parse.
*/


/*
You can intercept all the received data from this module with the method dataReceived(data).
Depending on the Protocol you chose, the nature of the data passed in this function will be different.
*/
function dataEvent(data, requestURL)
{
	script.log("Data received, request URL :"+requestURL+"\nContent :\n" +data);
}