/*
  ==============================================================================

    OSCInput.h
    Created: 29 Oct 2016 7:07:07pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef OSCINPUT_H_INCLUDED
#define OSCINPUT_H_INCLUDED

#include "JuceHeader.h"
#include "Input.h"

class OSCInput :
	public Input,
	public OSCReceiver::Listener<OSCReceiver::MessageLoopCallback>
	//public OSCReceiver::Listener<OSCReceiver::RealtimeCallback> //must change with that later !
{
public:
	OSCInput(const String &name = "OSC Input");
	~OSCInput() {}

	IntParameter * localPort;
	BoolParameter * isConnected;

	OSCReceiver receiver;
	void setupReceiver();

	float getFloatArg(OSCArgument a);
	int getIntArg(OSCArgument a);
	String getStringArg(OSCArgument a);

	void processMessage(const OSCMessage & msg);
	virtual void processMessageInternal(const OSCMessage &) {}

	virtual void onContainerParameterChanged(Parameter * p);
	virtual void oscMessageReceived(const OSCMessage & message) override;
};



#endif  // OSCINPUT_H_INCLUDED
