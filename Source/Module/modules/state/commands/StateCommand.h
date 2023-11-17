/*
  ==============================================================================

	StateCommand.h
	Created: 20 Feb 2017 2:12:36pm
	Author:  Ben

  ==============================================================================
*/

#pragma once

class StateModule;

class StateCommand :
	public BaseCommand
{
public:
	StateCommand(StateModule* _module, CommandContext context, var params, Multiplex* multiplex = nullptr);
	virtual ~StateCommand();

	enum ActionType {
		SET_STATE_ACTIVATION,
		TOGGLE_STATE,
		TRIGGER_ACTION,
		SET_ACTION_ENABLED,
		TOGGLE_ACTION,
		SET_TOGGLE_STATE,
		SET_MAPPING_ENABLED,
		TOGGLE_MAPPING,
		SET_SEQUENTIAL_CONDITION_INDEX,
		CANCEL_DELAYED_CONSEQUENCE, 
		CANCEL_STATE_DELAYED_CONSEQUENCES, 
		CANCEL_ALL_DELAYED_CONSEQUENCES,
		CONDUCTOR_NEXT_TRIGGER,
		CONDUCTOR_SET_CUE_INDEX,
		CONDUCTOR_TRIGGER_INDEX
	};

	ActionType actionType;
	StateModule* stateModule;

	TargetParameter* target;
	Parameter* val;

	void triggerInternal(int multiplexIndex) override;

	static BaseCommand* create(ControllableContainer* module, CommandContext context, var params, Multiplex* multiplex) { return new StateCommand((StateModule*)module, context, params, multiplex); }

};