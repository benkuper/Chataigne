/*
  ==============================================================================

    TimeTrigger.cpp
    Created: 20 Nov 2016 3:18:20pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "Common/Processor/ProcessorIncludes.h"

ChataigneTimeTrigger::ChataigneTimeTrigger(StringRef name) :
	TimeTrigger(name)
{ 
	csm.reset(new ConsequenceManager("Trigger consequences"));
	untcsm.reset(new ConsequenceManager("Untrigger consequences"));
	addChildControllableContainer(csm.get());
	addChildControllableContainer(untcsm.get());
}

ChataigneTimeTrigger::~ChataigneTimeTrigger()
{
}

void ChataigneTimeTrigger::onContainerParameterChangedInternal(Parameter* p)
{
	TimeTrigger::onContainerParameterChangedInternal(p);

	if (p == enabled)
	{
		csm->setForceDisabled(!enabled->boolValue());
		untcsm->setForceDisabled(!enabled->boolValue());
	}
}

void ChataigneTimeTrigger::triggerInternal()
{
	csm->triggerAll();
}

void ChataigneTimeTrigger::unTriggerInternal()
{
	untcsm->triggerAll();
}

var ChataigneTimeTrigger::getJSONData(bool includeNonOverriden)
{
	var data = TimeTrigger::getJSONData(includeNonOverriden);
	data.getDynamicObject()->setProperty("consequences", csm->getJSONData());
	data.getDynamicObject()->setProperty("untriggerConsequences", untcsm->getJSONData());
	return data;
}

void ChataigneTimeTrigger::loadJSONDataInternal(var data)
{
	TimeTrigger::loadJSONDataInternal(data);
	csm->loadJSONData(data.getProperty("consequences", var()));
	untcsm->loadJSONData(data.getProperty("untriggerConsequences", var()));
}
