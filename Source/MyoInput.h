/*
  ==============================================================================

    MyoInput.h
    Created: 29 Oct 2016 7:12:00pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef MYOINPUT_H_INCLUDED
#define MYOINPUT_H_INCLUDED


#include "Input.h"

class MyoInput :
	public Input
{
public:
	MyoInput();
	~MyoInput() {}

	static MyoInput * create() { return new MyoInput(); }
	virtual String getTypeString() const override { return "Myo"; }
};

#endif  // MYOINPUT_H_INCLUDED
