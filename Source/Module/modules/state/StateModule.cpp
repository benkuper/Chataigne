/*
  ==============================================================================

    StateModule.cpp
    Created: 20 Feb 2017 2:12:28pm
    Author:  Ben

  ==============================================================================
*/



StateModule::StateModule(StateManager * _manager) :
	Module("State Machine"),
	manager(_manager)
{
	defManager->add(getBaseStateCommand("State", "Set State Activation", CommandContext::BOTH, StateCommand::SET_STATE_ACTIVATION));
	defManager->add(getBaseStateCommand("State", "Toggle State", CommandContext::BOTH, StateCommand::TOGGLE_STATE));
	defManager->add(getBaseStateCommand("State", "Cancel Delayed Consequences", CommandContext::BOTH, StateCommand::CANCEL_STATE_DELAYED_CONSEQUENCES));

	defManager->add(getBaseStateCommand("Action", "Trigger Action", CommandContext::BOTH, StateCommand::TRIGGER_ACTION));
	defManager->add(getBaseStateCommand("Action", "Set Action Enabled", CommandContext::BOTH, StateCommand::SET_ACTION_ENABLED));
	defManager->add(getBaseStateCommand("Action", "Toggle Action", CommandContext::BOTH, StateCommand::TOGGLE_ACTION));
	defManager->add(getBaseStateCommand("Action", "Set Condition Toggle State", CommandContext::BOTH, StateCommand::SET_TOGGLE_STATE));
	defManager->add(getBaseStateCommand("Action", "Set Sequential Condition Index", CommandContext::BOTH, StateCommand::SET_SEQUENTIAL_CONDITION_INDEX));
	defManager->add(getBaseStateCommand("Action", "Cancel Delayed Consequence", CommandContext::BOTH, StateCommand::CANCEL_DELAYED_CONSEQUENCE));

	defManager->add(getBaseStateCommand("Mapping", "Set Mapping Enabled", CommandContext::BOTH, StateCommand::SET_MAPPING_ENABLED));
	defManager->add(getBaseStateCommand("Mapping", "Toggle Mapping", CommandContext::BOTH, StateCommand::TOGGLE_MAPPING));

	defManager->add(getBaseStateCommand("Conductor", "Trigger Next Cue", CommandContext::BOTH, StateCommand::CONDUCTOR_NEXT_TRIGGER));
	defManager->add(getBaseStateCommand("Conductor", "Set Next Cue Index", CommandContext::BOTH, StateCommand::CONDUCTOR_SET_CUE_INDEX));


	defManager->add(getBaseStateCommand("", "Cancel All Delayed Consequences", CommandContext::BOTH, StateCommand::CANCEL_ALL_DELAYED_CONSEQUENCES));

}

StateModule::~StateModule()
{
}

CommandDefinition * StateModule::getBaseStateCommand(const String & menu, const String & commandName, CommandContext context, StateCommand::ActionType actionType)
{
	return CommandDefinition::createDef(this, menu,commandName, &StateCommand::create, context)->addParam("type", actionType);

}

