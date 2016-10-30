/*
  ==============================================================================

    DMXInput.h
    Created: 30 Oct 2016 1:42:41pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef DMXINPUT_H_INCLUDED
#define DMXINPUT_H_INCLUDED

#include "Input.h"

class DMXInput :
	public Input
{
public:
	DMXInput();
	~DMXInput() {}

	static DMXInput * create() { return new DMXInput(); }
};

#endif  // DMXINPUT_H_INCLUDED
