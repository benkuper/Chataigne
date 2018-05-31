/*
  ==============================================================================

    CommandFactory.h
    Created: 3 Nov 2016 12:24:41pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef COMMANDFACTORY_H_INCLUDED
#define COMMANDFACTORY_H_INCLUDED


#include "CommandDefinition.h"

class Module;

class CommandFactory
{
public:
	static CommandDefinition * showMenuAndGetCommand(CommandContext context, Module * lockedModule = nullptr);
};


#endif  // COMMANDFACTORY_H_INCLUDED
