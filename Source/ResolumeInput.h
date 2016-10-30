/*
  ==============================================================================

    ResolumeInput.h
    Created: 29 Oct 2016 7:07:11pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef RESOLUMEINPUT_H_INCLUDED
#define RESOLUMEINPUT_H_INCLUDED

#include "OSCInput.h"

class ResolumeInput :
	public OSCInput
{
public:
	ResolumeInput();
	~ResolumeInput() {}

	static ResolumeInput * create() { return new ResolumeInput(); }
};


#endif  // RESOLUMEINPUT_H_INCLUDED
