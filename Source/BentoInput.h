/*
  ==============================================================================

    BentoInput.h
    Created: 29 Oct 2016 7:16:27pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef BENTOINPUT_H_INCLUDED
#define BENTOINPUT_H_INCLUDED

#include "OSCInput.h"

class BentoInput :
	public OSCInput
{
public:
	BentoInput();
	~BentoInput() {}

	static BentoInput * create() { return new BentoInput(); }
};

#endif  // BENTOINPUT_H_INCLUDED
