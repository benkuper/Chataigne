/*
  ==============================================================================

    StateModule.h
    Created: 20 Feb 2017 2:12:28pm
    Author:  Ben

  ==============================================================================
*/

#pragma once 


#include "Module/Module.h"
#include "commands/StateCommand.h"

class StateManager;

class StateModule :
	public Module
{
public:
	StateModule(StateManager * manager);
	~StateModule();

	StateManager * manager;

	CommandDefinition * getBaseStateCommand(const String &menu, const String &commandName, CommandContext context, StateCommand::ActionType actionType);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StateModule)
};