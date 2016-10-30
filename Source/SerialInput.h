/*
  ==============================================================================

    SerialInput.h
    Created: 29 Oct 2016 7:09:30pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef SERIALINPUT_H_INCLUDED
#define SERIALINPUT_H_INCLUDED

#include "Input.h"

class SerialInput :
	public Input
{
public:
	SerialInput();
	~SerialInput() {}

	static SerialInput * create() { return new SerialInput(); }
};


#endif  // SERIALINPUT_H_INCLUDED
