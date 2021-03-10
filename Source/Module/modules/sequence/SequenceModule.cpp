/*
  ==============================================================================

    SequenceModule.cpp
    Created: 20 Feb 2017 2:12:01pm
    Author:  Ben

  ==============================================================================
*/

#include "TimeMachine/ChataigneSequenceManager.h"

SequenceModule::SequenceModule(ChataigneSequenceManager * _manager) :
	Module("Sequences"),
	manager(_manager)
{
	defManager->add(createBaseDefinition("Playback", "Play Sequence", CommandContext::ACTION, SequenceCommand::PLAY_SEQUENCE));
	defManager->add(createBaseDefinition("Playback", "Play Sequence At Index", CommandContext::ACTION, SequenceCommand::PLAY_SEQUENCE_AT));
	defManager->add(createBaseDefinition("Playback","Play Multi Sequences", CommandContext::ACTION,SequenceCommand::PLAY_MULTI_SEQUENCES));
	defManager->add(createBaseDefinition("Playback", "Pause Sequence", CommandContext::ACTION,SequenceCommand::PAUSE_SEQUENCE));
	defManager->add(createBaseDefinition("Playback", "Stop Sequence", CommandContext::ACTION, SequenceCommand::STOP_SEQUENCE));
	defManager->add(createBaseDefinition("Playback", "Stop All Sequences", CommandContext::ACTION,SequenceCommand::STOP_ALL_SEQUENCES));
	defManager->add(createBaseDefinition("Playback", "Toggle Sequence", CommandContext::ACTION,SequenceCommand::TOGGLE_SEQUENCE));
	defManager->add(createBaseDefinition("Time", "Set Current Time", CommandContext::BOTH, SequenceCommand::SET_TIME));
	defManager->add(createBaseDefinition("Time", "Move Time", CommandContext::ACTION,SequenceCommand::MOVE_TIME));
	defManager->add(createBaseDefinition("Time", "Go to cue", CommandContext::ACTION, SequenceCommand::GOTO_CUE));
	defManager->add(createBaseDefinition("Layer", "Enable Layer",CommandContext::ACTION, SequenceCommand::ENABLE_LAYER));
	defManager->add(createBaseDefinition("Layer", "Disable layer", CommandContext::ACTION, SequenceCommand::DISABLE_LAYER));
	defManager->add(createBaseDefinition("Layer", "Toggle layer", CommandContext::ACTION, SequenceCommand::TOGGLE_LAYER));
	defManager->add(CommandDefinition::createDef(this, "Audio", "Set Layer Volume", &SequenceAudioCommand::create, CommandContext::BOTH)->addParam("type", SequenceAudioCommand::SET_VOLUME));
	defManager->add(CommandDefinition::createDef(this, "Trigger","Set Trigger Enable", &SequenceCommand::create, CommandContext::BOTH)->addParam("type", SequenceCommand::SET_TRIGGER_ENABLED));
}

SequenceModule::~SequenceModule()
{
}

CommandDefinition * SequenceModule::createBaseDefinition(const String & menu, const String & type, CommandContext context, SequenceCommand::ActionType actionType)
{
	return CommandDefinition::createDef(this, menu, type, &SequenceCommand::create, context)->addParam("type", actionType);
}
