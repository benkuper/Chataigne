/*
  ==============================================================================

    OSCInput.h
    Created: 29 Oct 2016 7:07:07pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef OSCINPUT_H_INCLUDED
#define OSCINPUT_H_INCLUDED

#include "Input.h"

class OSCInput :
	public Input
{
public:
	OSCInput(const String &name = "OSC Input");
	~OSCInput() {}
};



#endif  // OSCINPUT_H_INCLUDED
