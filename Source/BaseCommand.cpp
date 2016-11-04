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
	params(_params),
	context(_context)
{
}


BaseCommand::~BaseCommand()
{
}

void BaseCommand::trigger()
{
}

void BaseCommand::setValue(var value)
{
}
