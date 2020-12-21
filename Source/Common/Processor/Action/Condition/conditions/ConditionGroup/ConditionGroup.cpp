/*
  ==============================================================================

    ConditionGroup.cpp
    Created: 21 Feb 2017 11:27:53am
    Author:  Ben

  ==============================================================================
*/

#include "ConditionGroup.h"

ConditionGroup::ConditionGroup(var params, IteratorProcessor* iterator) :
	Condition(ConditionGroup::getTypeStringStatic(), params, iterator),
	manager(iterator)
{
	addChildControllableContainer(&manager);
	manager.addConditionManagerListener(this);
}

ConditionGroup::~ConditionGroup()
{
}

void ConditionGroup::conditionManagerValidationChanged(ConditionManager *, int iterationIndex)
{
	setValid(iterationIndex, manager.getIsValid(iterationIndex, false));
}

var ConditionGroup::getJSONData()
{
	var data = Condition::getJSONData();
	data.getDynamicObject()->setProperty("manager", manager.getJSONData());
	return data;
}

void ConditionGroup::loadJSONDataInternal(var data)
{
	Condition::loadJSONDataInternal(data);
	manager.loadJSONData(data.getProperty("manager", var()));
}
