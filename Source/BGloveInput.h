/*
  ==============================================================================

    BGloveInput.h
    Created: 29 Oct 2016 7:11:49pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef BGLOVEINPUT_H_INCLUDED
#define BGLOVEINPUT_H_INCLUDED

#include "OSCInput.h"

class BGloveInput :
	public OSCInput
{
public:
	BGloveInput();
	~BGloveInput() {}


	static BGloveInput * create() { return new BGloveInput(); }
	virtual String getTypeString() const override { return "BGlove"; }
};

#endif  // BGLOVEINPUT_H_INCLUDED
