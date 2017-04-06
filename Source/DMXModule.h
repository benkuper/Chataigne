/*
  ==============================================================================

    DMXModule.h
    Created: 6 Apr 2017 10:22:10pm
    Author:  Ben

  ==============================================================================
*/

#ifndef DMXMODULE_H_INCLUDED
#define DMXMODULE_H_INCLUDED

//#include "DMXManager.h"
#include "SerialModule.h"

class DMXModule :
	public SerialModule,
	public Thread
{
public:
	DMXModule();
	~DMXModule();

	uint8 dmxData[513];
	void sendDMXValue(int channel, int value);

	void setCurrentPort(SerialDevice * port) override;

	static DMXModule * create() { return new DMXModule(); }
	virtual String getDefaultTypeString() const override { return "DMX"; }

	//InspectableEditor * getEditor(bool isRoot) override;

	// Inherited via Thread
	virtual void run() override;
};


#endif  // DMXMODULE_H_INCLUDED
