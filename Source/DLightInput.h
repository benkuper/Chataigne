/*
  ==============================================================================

    DLightInput.h
    Created: 29 Oct 2016 7:07:30pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef DLIGHTINPUT_H_INCLUDED
#define DLIGHTINPUT_H_INCLUDED

#include "OSCInput.h"

class DLightInput :
	public OSCInput
{
public:
	DLightInput();
	~DLightInput() {}


	static DLightInput * create() { return new DLightInput(); }
};


#endif  // DLIGHTINPUT_H_INCLUDED
