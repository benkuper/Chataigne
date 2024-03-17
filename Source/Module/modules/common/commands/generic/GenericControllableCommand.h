/*
  ==============================================================================

	GenericControllableCommand.h
	Created: 25 May 2021 10:34:11am
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class GenericControllableCommand :
	public BaseCommand,
	public EngineListener
{
public:

	enum Action { SET_VALUE, GO_TO_VALUE, TRIGGER, SET_ENABLED };

	GenericControllableCommand(Module* _module, CommandContext context, var params, Multiplex* multiplex = nullptr);
	virtual ~GenericControllableCommand();

	enum Operator { EQUAL, INVERSE, ADD, SUBTRACT, MULTIPLY, DIVIDE, MAX, MIN, NEXT_ENUM, PREV_ENUM, RANDOM };

	Action action;
	TargetParameter* target;

	EnumParameter* valueOperator;
	EnumParameter* componentOperator;
	BoolParameter* loop;
	BoolParameter* randomAlwaysUnique;

	WeakReference<Parameter> value;
	var dataToLoad;
	var ghostValueData; // to keep when target is lost
	var ghostValueParamLinkData;
	var ghostOperator;
	var ghostComponent;

	FloatParameter* time;
	std::unique_ptr<Automation> automation;

	bool isUpdatingContent; //to avoid sending stuff when just updating target or component


	virtual void updateComponentFromTarget();
	virtual void updateValueFromTargetAndComponent();

	virtual Controllable* getControllableFromTarget();

	virtual void updateOperatorOptions();

	virtual void triggerInternal(int multiplexIndex) override;
	virtual Controllable* getTargetControllableAtIndex(int multiplexIndex);

	virtual void linkUpdated(ParameterLink* pLink) override;
	virtual void onContainerParameterChanged(Parameter*) override;

	virtual void loadJSONDataInternal(var data) override;
	virtual void endLoadFile() override;
	virtual void loadGhostData(var data);

	static bool checkEnableTargetFilter(Controllable* c);

	//void changeListenerCallback(ChangeBroadcaster* source) override;

	static BaseCommand* create(ControllableContainer* module, CommandContext context, var params, Multiplex* multiplex = nullptr);
};
