/*
  ==============================================================================

    TimeTrigger.cpp
    Created: 20 Nov 2016 3:18:20pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "TimeAction.h"

TimeAction::TimeAction(StringRef name) :
	TimeTrigger(name)
{
	addChildControllableContainer(&csm);
}

TimeAction::~TimeAction()
{
}

void TimeAction::onContainerParameterChangedInternal(Parameter* p)
{
	TimeTrigger::onContainerParameterChangedInternal(p);
	if (p == enabled)
	{
		csm.setForceDisabled(!enabled->boolValue());
	}
}

void TimeAction::triggerInternal()
{
	csm.triggerAll->trigger();
}

var TimeAction::getJSONData()
{
	var data = TimeTrigger::getJSONData();
	data.getDynamicObject()->setProperty("consequences", csm.getJSONData());
	return data;
}

void TimeAction::loadJSONDataInternal(var data)
{
	TimeTrigger::loadJSONDataInternal(data);
	csm.loadJSONData(data.getProperty("consequences", var()));
}
