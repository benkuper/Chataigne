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
	StateCommand(StateModule * _module, CommandContext context, var params, IteratorProcessor * iterator = nullptr);
	virtual ~StateCommand();

	enum ActionType { SET_STATE_ACTIVATION, TOGGLE_STATE, TRIGGER_ACTION, SET_ACTION_ENABLED, TOGGLE_ACTION, SET_TOGGLE_STATE, SET_MAPPING_ENABLED, TOGGLE_MAPPING };

	ActionType actionType;
	StateModule * stateModule;

	TargetParameter * target;
	BoolParameter* enableVal;

	void triggerInternal(int iterationIndex) override;


	//Delayed loading mechanism to ensure all content is created for right targeting
	var dataToLoad;

	void loadJSONDataInternal(var data) override;
	void endLoadFile() override;

	static BaseCommand * create(ControllableContainer * module, CommandContext context, var params, IteratorProcessor * iterator) { return new StateCommand((StateModule *)module, context, params, iterator); }

};