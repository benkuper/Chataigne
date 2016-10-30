/*
  ==============================================================================

    KinectInput.h
    Created: 29 Oct 2016 7:12:49pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef KINECTINPUT_H_INCLUDED
#define KINECTINPUT_H_INCLUDED

#include "Input.h"

class KinectInput :
	public Input
{
public:
	KinectInput();
	~KinectInput() {}

	static KinectInput * create() { return new KinectInput(); }
	virtual String getTypeString() const override { return "Kinect"; }

};

#endif  // KINECTINPUT_H_INCLUDED
