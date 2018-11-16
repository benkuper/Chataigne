/* ********** STREAMING MODULE (UDP, SERIAL) SPECIFIC SCRIPTING ********************* */
/*

Streaming Modules (i.e. UDP and Serial Module) have specific methods that can be used to handle receiving and sendin data over the connection.
With streaming modules, there are 2 ways of sending data : either as a UTF-8 String or as separate bytes

local.send("This is my message"); //This will send the string passed in as ASCII characters 
local.sendBytes(30,210,46,255,10); //This will send all the bytes passed in as they are

*/

/*
You can intercept all the received data from this module with the method dataReceived(data).
Depending on the Protocol you chose, the nature of the data passed in this function will be different.
*/

function dataReceived(data)
{
	//If mode is "Lines", you can expect data to be a single line String
	script.log("Data received : " +data);

	//If mode is anything else, you can expect data to be an array of bytes
	script.log("Bytes received : "+data.length);
	for(var i=0; i < data.length; i++)
	{
		script.log(" > " + data[i]);
	}
}