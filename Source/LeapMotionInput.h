/*
  ==============================================================================

    LeapMotionInput.h
    Created: 29 Oct 2016 7:15:41pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef LEAPMOTIONINPUT_H_INCLUDED
#define LEAPMOTIONINPUT_H_INCLUDED


#include "Input.h"

class LeapMotionInput :
	public Input
{
public:
	LeapMotionInput();
	~LeapMotionInput() {}

	static LeapMotionInput * create() { return new LeapMotionInput(); }
};



#endif  // LEAPMOTIONINPUT_H_INCLUDED
