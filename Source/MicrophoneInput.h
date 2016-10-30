/*
  ==============================================================================

    MicrophoneInput.h
    Created: 29 Oct 2016 7:22:49pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef MICROPHONEINPUT_H_INCLUDED
#define MICROPHONEINPUT_H_INCLUDED



#include "Input.h"

class MicrophoneInput :
	public Input
{
public:
	MicrophoneInput();
	~MicrophoneInput() {}

	static MicrophoneInput * create() { return new MicrophoneInput(); }
	virtual String getTypeString() const override { return "Microphone"; }
};


#endif  // MICROPHONEINPUT_H_INCLUDED
