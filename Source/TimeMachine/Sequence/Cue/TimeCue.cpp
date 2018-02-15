/*
  ==============================================================================

    TimeCue.cpp
    Created: 6 Apr 2017 11:13:23am
    Author:  Ben

  ==============================================================================
*/

#include "TimeCue.h"
#include "../Sequence.h"

TimeCue::TimeCue(const float & _time) :
	BaseItem("Time Cue")
{
	//nameParam->hideInEditor = false;

	time = addFloatParameter("Time", "Cue Time", _time, 0, 3600);
	time->defaultUI = FloatParameter::TIME;

	helpID = "TimeCue";
}

TimeCue::~TimeCue()
{
}

Sequence * TimeCue::getSequence()
{
	ControllableContainer * cc = parentContainer;

	while (cc != nullptr)
	{
		Sequence * s = dynamic_cast<Sequence *>(cc);
		if (s != nullptr) return s;
		cc = cc->parentContainer;
	}

	return nullptr;
}
