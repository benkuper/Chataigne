/*
  ==============================================================================

    OSCOutput.cpp
    Created: 29 Oct 2016 7:21:12pm
    Author:  bkupe

  ==============================================================================
*/

#include "OSCOutput.h"

OSCOutput::OSCOutput(const String & name) :
	Output(name)
{
	useLocal = addBoolParameter("Local", "Send to Local IP (127.0.0.1). Allow to quickly switch between local and remote IP.", true);
	remoteHost = addStringParameter("Remote Host", "Remote Host to send to.", "127.0.0.1");
	remotePort = addIntParameter("Remote port", "Port on which the remote host is listening to", 9000,1024,65535);

	setupSender();
}

OSCOutput::~OSCOutput()
{
}

void OSCOutput::setupSender()
{
	sender.connect(remoteHost->stringValue(), remotePort->intValue());
}

void OSCOutput::sendOSC(const OSCMessage & msg)
{
	DBG("OSC Output :: Send osc " << msg.getAddressPattern().toString());
	sender.send(msg);
}

void OSCOutput::onContainerParameterChanged(Parameter * p)
{
	if (p == remoteHost || p == remotePort) setupSender();
}
