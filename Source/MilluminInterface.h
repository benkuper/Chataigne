/*
  ==============================================================================

    MilluminInput.h
    Created: 29 Oct 2016 7:07:18pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef MILLUMININPUT_H_INCLUDED
#define MILLUMININPUT_H_INCLUDED

#include "OSCInput.h"

class MilluminInput :
	public OSCInput
{
public:
	MilluminInput();
	~MilluminInput() {}

	static MilluminInput * create() { return new MilluminInput(); }
	virtual String getTypeString() const override { return "Millumin"; }
};

#endif  // MILLUMININPUT_H_INCLUDED
