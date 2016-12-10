/*
  ==============================================================================

    TimeTrigger.h
    Created: 20 Nov 2016 3:18:20pm
    Author:  Ben Kuper

  ==============================================================================
*/

#ifndef TIMETRIGGER_H_INCLUDED
#define TIMETRIGGER_H_INCLUDED

#include "Action.h"

class TimeTrigger :
	public Action
{
public:
	TimeTrigger(float time = 0, const String &name = "Trigger");
	virtual ~TimeTrigger();

	FloatParameter * time;
	bool isTriggered;
	
	//ui
	FloatParameter * flagY;

};



#endif  // TIMETRIGGER_H_INCLUDED
