/*
  ==============================================================================

    StateModule.cpp
    Created: 20 Feb 2017 2:12:28pm
    Author:  Ben

  ==============================================================================
*/

#include "StateModule.h"
#include "StateManager.h"
#include "CommandDefinition.h"
#include "StateCommand.h"

StateModule::StateModule(StateManager * _manager) :
	Module("State Machine"),
	manager(_manager)
{
	defManager.add(CommandDefinition::createDef(this, "State", "Activate State", &StateCommand::create, CommandContext::ACTION)->addParam("type", StateCommand::ACTIVATE_STATE));
	defManager.add(CommandDefinition::createDef(this, "State", "Deactivate State", &StateCommand::create, CommandContext::ACTION)->addParam("type", StateCommand::DEACTIVATE_STATE));
	defManager.add(CommandDefinition::createDef(this, "State", "Toggle State", &StateCommand::create, CommandContext::ACTION)->addParam("type", StateCommand::TOGGLE_STATE));

	defManager.add(CommandDefinition::createDef(this, "Action", "Trigger Action", &StateCommand::create, CommandContext::ACTION)->addParam("type", StateCommand::TRIGGER_ACTION));
	defManager.add(CommandDefinition::createDef(this, "Action", "Enable Action", &StateCommand::create, CommandContext::ACTION)->addParam("type", StateCommand::ENABLE_ACTION));
	defManager.add(CommandDefinition::createDef(this, "Action", "Disable Action", &StateCommand::create, CommandContext::ACTION)->addParam("type", StateCommand::DISABLE_ACTION));
	defManager.add(CommandDefinition::createDef(this, "Action", "Toggle Action", &StateCommand::create, CommandContext::ACTION)->addParam("type", StateCommand::TOGGLE_ACTION));

	defManager.add(CommandDefinition::createDef(this, "Mapping", "Enable Mapping", &StateCommand::create, CommandContext::ACTION)->addParam("type", StateCommand::ENABLE_MAPPING));
	defManager.add(CommandDefinition::createDef(this, "Mapping", "Disable Mapping", &StateCommand::create, CommandContext::ACTION)->addParam("type", StateCommand::DISABLE_MAPPING));
	defManager.add(CommandDefinition::createDef(this, "Mapping", "Toggle Mapping", &StateCommand::create, CommandContext::ACTION)->addParam("type", StateCommand::TOGGLE_MAPPING));
}

StateModule::~StateModule()
{
}

