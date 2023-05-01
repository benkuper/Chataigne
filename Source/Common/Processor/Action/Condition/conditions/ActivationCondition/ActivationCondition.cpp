/*
  ==============================================================================

    ActivationCondition.cpp
    Created: 30 May 2018 9:36:05am
    Author:  Ben

  ==============================================================================
*/

#include "Common/Processor/ProcessorIncludes.h"

ActivationCondition::ActivationCondition(var params, Multiplex* multiplex) :
	Condition(getTypeStringStatic((Type)(int)params.getProperty("type",ON_ACTIVATE)), params, multiplex),
	type((Type)(int)params.getProperty("type", ON_ACTIVATE))
{
}

ActivationCondition::~ActivationCondition()
{
}

var ActivationCondition::getJSONData()
{
	var data = Condition::getJSONData();
	data.getDynamicObject()->setProperty("activationType", type);
	return data;
}

void ActivationCondition::loadJSONDataInternal(var data)
{
	Condition::loadJSONDataInternal(data);
	type = (Type)(int)data.getProperty("activationType", ON_ACTIVATE);
}
