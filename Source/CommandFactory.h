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

class CommandFactory
{
public:
	static CommandDefinition * showMenuAndGetCommand();
};


#endif  // COMMANDFACTORY_H_INCLUDED
