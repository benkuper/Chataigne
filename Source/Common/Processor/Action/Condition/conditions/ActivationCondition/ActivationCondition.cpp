/*
  ==============================================================================

    ActivationCondition.cpp
    Created: 30 May 2018 9:36:05am
    Author:  Ben

  ==============================================================================
*/

#include "ActivationCondition.h"

ActivationCondition::ActivationCondition(var params) :
	Condition(getTypeStringStatic((Type)(int)params.getProperty("type",ON_ACTIVATE))),
	type((Type)(int)params.getProperty("type", ON_ACTIVATE))
{
	isValid->setValue(true);
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
