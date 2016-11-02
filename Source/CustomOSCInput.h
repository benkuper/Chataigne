/*
  ==============================================================================

    CustomOSCInput.h
    Created: 29 Oct 2016 7:07:54pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef CUSTOMOSCINPUT_H_INCLUDED
#define CUSTOMOSCINPUT_H_INCLUDED

#include "OSCInput.h"

class CustomOSCInput :
	public OSCInput
{
public:
	CustomOSCInput();
	~CustomOSCInput() {}

	BoolParameter * autoAdd;

	void processMessageInternal(const OSCMessage &msg) override;

	static CustomOSCInput * create() { return new CustomOSCInput(); }
	virtual String getTypeString() const override { return "OSC"; }
};

#endif  // CUSTOMOSCINPUT_H_INCLUDED
