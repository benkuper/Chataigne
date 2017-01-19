/*
  ==============================================================================

    OSCModule.h
    Created: 29 Oct 2016 7:07:07pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef OSCMODULE_H_INCLUDED
#define OSCMODULE_H_INCLUDED

#include "JuceHeader.h"
#include "Module.h"

class OSCModule :
	public Module,
	public OSCReceiver::Listener<OSCReceiver::RealtimeCallback>
{ 
public:
	OSCModule(const String &name = "OSC Module");
	~OSCModule() {}

	//RECEIVE
	IntParameter * localPort;
	BoolParameter * isConnected;
	OSCReceiver receiver;
	
	//SEND
	BoolParameter * useLocal;
	StringParameter * remoteHost;
	IntParameter * remotePort;
	OSCSender sender;

	//RECEIVE
	void setupReceiver();
	float getFloatArg(OSCArgument a);
	int getIntArg(OSCArgument a);
	String getStringArg(OSCArgument a);

	void processMessage(const OSCMessage & msg);
	virtual void processMessageInternal(const OSCMessage &) {}


	//SEND
	void setupSender();
	void sendOSC(const OSCMessage &msg);


	virtual void onContainerParameterChangedInternal(Parameter * p) override;
	virtual void oscMessageReceived(const OSCMessage & message) override;
};



#endif  // OSCMODULE_H_INCLUDED
