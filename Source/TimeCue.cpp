/*
  ==============================================================================

    TimeCue.cpp
    Created: 6 Apr 2017 11:13:23am
    Author:  Ben

  ==============================================================================
*/

#include "TimeCue.h"

TimeCue::TimeCue(const float & _time) :
	BaseItem("Cue")
{
	nameParam->hideInEditor = false;

	time = addFloatParameter("Time", "Cue Time", _time, 0, 3600);
	time->defaultUI = FloatParameter::TIME;

	helpID = "TimeCue";
}

TimeCue::~TimeCue()
{
}
