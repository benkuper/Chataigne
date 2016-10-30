/*
  ==============================================================================

    LiveInput.h
    Created: 29 Oct 2016 7:07:36pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef LIVEINPUT_H_INCLUDED
#define LIVEINPUT_H_INCLUDED

#include "OSCInput.h"

class LiveInput :
	public OSCInput
{
public:
	LiveInput();
	~LiveInput() {}

	static LiveInput * create() { return new LiveInput(); }
};


#endif  // LIVEINPUT_H_INCLUDED
