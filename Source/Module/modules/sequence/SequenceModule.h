/*
  ==============================================================================

    SequenceModule.h
    Created: 20 Feb 2017 2:12:01pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "Module/Module.h"
#include "commands/SequenceCommand.h"

class ChataigneSequenceManager;

class SequenceModule :
	public Module
{
public:
	SequenceModule(ChataigneSequenceManager * manager);
	~SequenceModule();

	ChataigneSequenceManager* manager;

	CommandDefinition * createBaseDefinition(const String &menu, const String &type, CommandContext context, SequenceCommand::ActionType actionType);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceModule)
};