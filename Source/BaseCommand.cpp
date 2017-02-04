/*
  ==============================================================================

    BaseCommand.cpp
    Created: 3 Nov 2016 10:50:32am
    Author:  bkupe

  ==============================================================================
*/

#include "BaseCommand.h"

BaseCommand::BaseCommand(ControllableContainer * _container, CommandContext _context, var _params) :
	ControllableContainer("Command"),
	container(_container),
	targetMappingParameter(nullptr),
	params(_params),
	context(_context)
{
}

void BaseCommand::setTargetMappingParameter(Parameter * p)
{
	if (targetMappingParameter != nullptr) targetMappingParameter->isControllableFeedbackOnly = false;
	targetMappingParameter = p;
	if (targetMappingParameter != nullptr) targetMappingParameter->isControllableFeedbackOnly = true;
}

void BaseCommand::setValue(var value)
{
	if (targetMappingParameter == nullptr) return;
	targetMappingParameter->setValue(value);
	trigger();
}
