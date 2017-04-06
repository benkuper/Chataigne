/*
  ==============================================================================

    SequenceModule.cpp
    Created: 20 Feb 2017 2:12:01pm
    Author:  Ben

  ==============================================================================
*/

#include "SequenceModule.h"
#include "SequenceManager.h"
#include "SequenceCommand.h"
#include "SequenceCueCommand.h"

SequenceModule::SequenceModule(SequenceManager * _manager) :
	Module("Sequence Manager"),
	manager(_manager)
{
	defManager.add(CommandDefinition::createDef(this, "Sequence", "Play Sequence", &SequenceCommand::create, CommandContext::ACTION)->addParam("type", SequenceCommand::PLAY_SEQUENCE));
	defManager.add(CommandDefinition::createDef(this, "Sequence", "Pause Sequence", &SequenceCommand::create, CommandContext::ACTION)->addParam("type", SequenceCommand::PAUSE_SEQUENCE));
	defManager.add(CommandDefinition::createDef(this, "Sequence", "Stop Sequence", &SequenceCommand::create, CommandContext::ACTION)->addParam("type", SequenceCommand::STOP_SEQUENCE));
	defManager.add(CommandDefinition::createDef(this, "Sequence", "Toggle Sequence", &SequenceCommand::create, CommandContext::ACTION)->addParam("type", SequenceCommand::TOGGLE_SEQUENCE));
	defManager.add(CommandDefinition::createDef(this, "Sequence", "Go to cue", &SequenceCueCommand::create, CommandContext::ACTION));

}

SequenceModule::~SequenceModule()
{
}
