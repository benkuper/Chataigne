/*
  ==============================================================================

    StateModule.cpp
    Created: 20 Feb 2017 2:12:28pm
    Author:  Ben

  ==============================================================================
*/


#include "StateModule.h"
#include "StateMachine/StateManager.h"
#include "Common/Command/CommandDefinition.h"
#include "commands/StateCommand.h"

StateModule::StateModule(StateManager * _manager) :
	Module("State Machine"),
	manager(_manager)
{
	defManager->add(getBaseStateCommand("State", "Activate State", CommandContext::ACTION, StateCommand::ACTIVATE_STATE));
	defManager->add(getBaseStateCommand("State", "Deactivate State", CommandContext::ACTION, StateCommand::DEACTIVATE_STATE));
	defManager->add(getBaseStateCommand("State", "Toggle State", CommandContext::ACTION, StateCommand::TOGGLE_STATE));

	defManager->add(getBaseStateCommand("Action", "Trigger Action", CommandContext::ACTION, StateCommand::TRIGGER_ACTION));
	defManager->add(getBaseStateCommand("Action", "Enable Action", CommandContext::ACTION, StateCommand::ENABLE_ACTION));
	defManager->add(getBaseStateCommand("Action", "Disable Action", CommandContext::ACTION, StateCommand::DISABLE_ACTION));
	defManager->add(getBaseStateCommand("Action", "Toggle Action", CommandContext::ACTION, StateCommand::TOGGLE_ACTION));

	defManager->add(getBaseStateCommand("Mapping", "Enable Mapping", CommandContext::ACTION, StateCommand::ENABLE_MAPPING));
	defManager->add(getBaseStateCommand("Mapping", "Disable Mapping", CommandContext::ACTION, StateCommand::DISABLE_MAPPING));
	defManager->add(getBaseStateCommand("Mapping", "Toggle Mapping", CommandContext::ACTION, StateCommand::TOGGLE_MAPPING));
}

StateModule::~StateModule()
{
}

CommandDefinition * StateModule::getBaseStateCommand(const String & menu, const String & commandName, CommandContext context, StateCommand::ActionType actionType)
{
	return CommandDefinition::createDef(this, menu,commandName, &StateCommand::create, context)->addParam("type", actionType);

}

