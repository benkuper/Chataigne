/*
  ==============================================================================

    OSCOutput.h
    Created: 29 Oct 2016 7:21:12pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef OSCOUTPUT_H_INCLUDED
#define OSCOUTPUT_H_INCLUDED

#include "Output.h"
class OSCOutput :
	public Output
{
public:
	OSCOutput(const String &name = "OSC Output");
	virtual ~OSCOutput();

	BoolParameter * useLocal;
	StringParameter * remoteHost;
	IntParameter * remotePort;

	OSCSender sender;

	void setupSender();
	void sendOSC(const OSCMessage &msg);

	void onContainerParameterChanged(Parameter *);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OSCOutput)
};



#endif  // OSCOUTPUT_H_INCLUDED
