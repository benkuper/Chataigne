/*
  ==============================================================================

    TimeTrigger.cpp
    Created: 20 Nov 2016 3:18:20pm
    Author:  Ben Kuper

  ==============================================================================
*/

ChataigneTimeTrigger::ChataigneTimeTrigger(StringRef name) :
	TimeTrigger(name)
{
	addChildControllableContainer(&csm);
}

ChataigneTimeTrigger::~ChataigneTimeTrigger()
{
}

void ChataigneTimeTrigger::onContainerParameterChangedInternal(Parameter* p)
{
	TimeTrigger::onContainerParameterChangedInternal(p);

	if (p == enabled)
	{
		csm.setForceDisabled(!enabled->boolValue());
	}
}

void ChataigneTimeTrigger::triggerInternal()
{
	csm.triggerAll();
}

var ChataigneTimeTrigger::getJSONData()
{
	var data = TimeTrigger::getJSONData();
	data.getDynamicObject()->setProperty("consequences", csm.getJSONData());
	return data;
}

void ChataigneTimeTrigger::loadJSONDataInternal(var data)
{
	TimeTrigger::loadJSONDataInternal(data);
	csm.loadJSONData(data.getProperty("consequences", var()));
}
