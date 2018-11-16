/* ********** OSC MODULE SPECIFIC SCRIPTING ********************* */
/*

OSC Modules have specific methods that can be used to send OSC message from Script.
If you want to send an OSC Message from this script, you can do the following :

local.send("/myAddress",1,.5f,"cool"); //This will send an OSC Message with address "/myAddress" and 3 arguments <int>, <float> and <string>

*/
/*
You can intercept OSC message with the function oscEvent(address, args)
*/

function oscEvent(address, args)
{
	//param "address" is the address of the OSC Message
	//param "args" is an array containing all the arguments of the OSC Message

	script.log("OSC Message received "+address+", "+args.length+" arguments");
	for(var i=0; i < args.length; i++)
	{
		script.log(" > "+args[i]);
	}
}