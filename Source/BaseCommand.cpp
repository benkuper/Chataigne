/*
  ==============================================================================

    BaseCommand.cpp
    Created: 3 Nov 2016 10:50:32am
    Author:  bkupe

  ==============================================================================
*/

#include "BaseCommand.h"

BaseCommand::BaseCommand(Module * _module, CommandContext _context, var _params) :
	ControllableContainer("Command"),
	module(_module),
	params(_params),
	context(_context)
{
}

void BaseCommand::setTargetMappingParameterAt(Parameter * p, int index)
{
	if (index < 0) return;
	if (index >= targetMappingParameters.size()) targetMappingParameters.resize(index + 1);
	Parameter * oldP = targetMappingParameters[index];

	if (oldP != nullptr) oldP->isControllableFeedbackOnly = false;
	targetMappingParameters.set(index,p);
	if (p != nullptr) p->isControllableFeedbackOnly = true;
}

void BaseCommand::clearTargetMappingParameters()
{
	for (int i = 0; i < targetMappingParameters.size(); i++) setTargetMappingParameterAt(nullptr, i);
	targetMappingParameters.clear();
}

void BaseCommand::setValue(var value)
{
	if (!value.isArray())
	{
		if (targetMappingParameters.size() > 0 && targetMappingParameters[0] != nullptr) targetMappingParameters[0]->setValue(value);
	}
	else
	{
		int maxSize = jmin(value.size(),targetMappingParameters.size());
		for (int i = 0; i < maxSize; i++)
		{
			if (targetMappingParameters[i] != nullptr) targetMappingParameters[i]->setValue(value[i]);
		}
	}

	trigger();
}
