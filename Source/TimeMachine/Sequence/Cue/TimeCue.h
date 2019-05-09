/*
  ==============================================================================

    TimeCue.h
    Created: 6 Apr 2017 11:13:23am
    Author:  Ben

  ==============================================================================
*/

#ifndef TIMECUE_H_INCLUDED
#define TIMECUE_H_INCLUDED

#include "JuceHeader.h"
class Sequence;

class TimeCue :
	public BaseItem
{
public:
	TimeCue(const float &_time = 0);
	~TimeCue();

	FloatParameter * time;
	BoolParameter * isLocked;
	BoolParameter * pauseOnCue;

	Sequence * getSequence();
};

#endif  // TIMECUE_H_INCLUDED
