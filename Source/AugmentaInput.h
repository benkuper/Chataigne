/*
  ==============================================================================

    AugmentaInput.h
    Created: 29 Oct 2016 7:13:49pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef AUGMENTAINPUT_H_INCLUDED
#define AUGMENTAINPUT_H_INCLUDED

#include "OSCInput.h"

class AugmentaInput :
	public OSCInput
{
public:
	AugmentaInput();
	~AugmentaInput() {}


	static AugmentaInput * create() { return new AugmentaInput(); }
};



#endif  // AUGMENTAINPUT_H_INCLUDED
