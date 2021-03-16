

/* ********** WEBSOCKET SERVER SPECIFIC SCRIPTING ********************* */
/*

Websoskets modules can be used as standard Streaming Module and use the dataReceived function above, 
but you can also intercept messages and data directly from the streaming, before it is processed, using specific 
event callbacks below.
Those callbacks include a "connectionId" argument that allows to differentiate the client that sent it.
*/

function wsMessageReceived(connectionId, message)
{
	script.log("Websocket data received from "+connectionId+" : " +message);
}

function wsDataReceived(connectionId, data)
{
	script.log("Websocket data received from "+connectionId+" : " +data);
}