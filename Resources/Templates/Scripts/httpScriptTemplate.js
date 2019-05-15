

/* ********** HTTP MODULE SPECIFIC SCRIPTING ********************* */
/*

HTTP Mdule has specific methods that can be used to handle receiving and sending data over the connection.
With HTTP modules, there are 2 ways of sending data : either via GET or POST

local.sendGET("https://httpbin.org/anything?myValue1=1&myValue2=super");
local.sendPOST("https://httpbin.org/anything","myValue1",1,"myValue2",2); //After address, values are taken 2 by 2 to form a name/value pair.

*/

/*
You can intercept all the received data from this module with the method dataReceived(data).
Depending on the Protocol you chose, the nature of the data passed in this function will be different.
*/

function dataEvent(data, requestURL)
{
	script.log("Data received, request URL :"+requestURL+"\nContent :\n" +data);
}