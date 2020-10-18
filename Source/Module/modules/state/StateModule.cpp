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
	defManager->add(getBaseStateCommand("State", "Set State Activation", CommandContext::ACTION, StateCommand::SET_STATE_ACTIVATION));
	defManager->add(getBaseStateCommand("State", "Toggle State", CommandContext::ACTION, StateCommand::TOGGLE_STATE));

	defManager->add(getBaseStateCommand("Action", "Trigger Action", CommandContext::ACTION, StateCommand::TRIGGER_ACTION));
	defManager->add(getBaseStateCommand("Action", "Set Action Enabled", CommandContext::ACTION, StateCommand::SET_ACTION_ENABLED));
	defManager->add(getBaseStateCommand("Action", "Toggle Action", CommandContext::ACTION, StateCommand::TOGGLE_ACTION));
	defManager->add(getBaseStateCommand("Action", "Set Condition Toggle State", CommandContext::ACTION, StateCommand::SET_TOGGLE_STATE));

	defManager->add(getBaseStateCommand("Mapping", "Set Mapping Enabled", CommandContext::ACTION, StateCommand::SET_MAPPING_ENABLED));
	defManager->add(getBaseStateCommand("Mapping", "Toggle Mapping", CommandContext::ACTION, StateCommand::TOGGLE_MAPPING));
}

StateModule::~StateModule()
{
}

CommandDefinition * StateModule::getBaseStateCommand(const String & menu, const String & commandName, CommandContext context, StateCommand::ActionType actionType)
{
	return CommandDefinition::createDef(this, menu,commandName, &StateCommand::create, context)->addParam("type", actionType);

}

