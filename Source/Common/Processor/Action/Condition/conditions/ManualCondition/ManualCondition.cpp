/*
  ==============================================================================

	ManualCondition.cpp
	Created: 1 May 2023 2:59:24pm
	Author:  bkupe

  ==============================================================================
*/

#include "Common/Processor/ProcessorIncludes.h"

ManualCondition::ManualCondition(var params, Multiplex* multiplex) :
	Condition(getTypeStringStatic(), params, multiplex)
{
	active = addBoolParameter("Active", "Sets the validation state of this action. Simple, efficient, no buzz no fuzz. Boom.", false);
}

ManualCondition::~ManualCondition()
{
}

void ManualCondition::onContainerParameterChangedInternal(Parameter* p)
{
	Condition::onContainerParameterChangedInternal(p);
	if (p == active)
	{
		for (int i = 0; i < getMultiplexCount(); i++) setValid(i, active->boolValue());
	}

}