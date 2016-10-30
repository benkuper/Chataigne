/*
  ==============================================================================

    KinectV2Input.h
    Created: 29 Oct 2016 7:12:54pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef KINECTV2INPUT_H_INCLUDED
#define KINECTV2INPUT_H_INCLUDED


#include "Input.h"

class KinectV2Input :
	public Input
{
public:
	KinectV2Input();
	~KinectV2Input() {}

	static KinectV2Input * create() { return new KinectV2Input(); }
	virtual String getTypeString() const override { return "KinectV2"; }
};



#endif  // KINECTV2INPUT_H_INCLUDED
