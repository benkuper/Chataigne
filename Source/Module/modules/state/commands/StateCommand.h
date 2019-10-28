/*
  ==============================================================================

    StateCommand.h
    Created: 20 Feb 2017 2:12:36pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "Common/Command/BaseCommand.h"

class StateModule;

class StateCommand :
	public BaseCommand,
	public EngineListener
{
public:
	StateCommand(StateModule * _module, CommandContext context, var params);
	virtual ~StateCommand();

	enum ActionType { ACTIVATE_STATE, DEACTIVATE_STATE, TOGGLE_STATE, TRIGGER_ACTION, ENABLE_ACTION, DISABLE_ACTION, TOGGLE_ACTION, ENABLE_MAPPING, DISABLE_MAPPING, TOGGLE_MAPPING };

	ActionType actionType;
	StateModule * stateModule;

	TargetParameter * target;

	void triggerInternal() override;


	//Delayed loading mechanism to ensure all content is created for right targeting
	var dataToLoad;

	void loadJSONDataInternal(var data) override;
	void endLoadFile() override;

	static BaseCommand * create(ControllableContainer * module, CommandContext context, var params) { return new StateCommand((StateModule *)module, context, params); }

};