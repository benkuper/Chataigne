/*
  ==============================================================================

    TimeTrigger.cpp
    Created: 20 Nov 2016 3:18:20pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "TimeTrigger.h"


TimeTrigger::TimeTrigger(float _time, const String &name) :
	Action(name)
{
	nameParam = addStringParameter("name", "Name of the trigger", name);
	time = addFloatParameter("Time", "Time at which the action is triggered", _time, 0, 3600);
	flagY = addFloatParameter("Flag Y", "Position of the trigger's flag", 0);
}

TimeTrigger::~TimeTrigger()
{

}
