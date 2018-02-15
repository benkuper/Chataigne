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
	context(_context),
	module(_module),
	params(_params)
{

}

void BaseCommand::setTargetMappingParameterAt(WeakReference<Parameter> p, int index)
{
	if (context != CommandContext::MAPPING) return;

	if (index < 0) return;
	if (index >= targetMappingParameters.size()) targetMappingParameters.resize(index + 1);
	WeakReference<Parameter> oldP = targetMappingParameters[index];

	if (oldP != nullptr && !oldP.wasObjectDeleted())
	{
		oldP->setControllableFeedbackOnly(false);
	}

	targetMappingParameters.set(index, p);

	if (p != nullptr && !p.wasObjectDeleted())
	{
		p->setControllableFeedbackOnly(true);
	}
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
	} else
	{
		int maxSize = jmin(value.size(), targetMappingParameters.size());
		for (int i = 0; i < maxSize; i++)
		{
			WeakReference<Parameter> p = targetMappingParameters[i];
			if (p != nullptr && !p.wasObjectDeleted())
			{
				if (p->value.isArray() && p->value.size() == value.size())
				{
					p->setValue(value);
				} else
				{
					p->setValue(value[i]);
				}
			}
		}
	}

	trigger();
}


void BaseCommand::trigger()
{

}
