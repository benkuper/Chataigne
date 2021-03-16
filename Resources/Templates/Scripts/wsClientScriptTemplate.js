

/* ********** STREAMING MODULE (UDP, TCP, SERIAL, WEBSOCKET) SPECIFIC SCRIPTING ********************* */
/*

Websoskets modules can be used as standard Streaming Module and use the dataReceived function above, 
but you can also intercept messages and data directly from the streaming, before it is processed, using specific 
event callbacks below
*/

function wsMessageReceived(message)
{
	script.log("Websocket data received : " +message);
}

function wsDataReceived(data)
{
	script.log("Websocket data received : " +data);
}