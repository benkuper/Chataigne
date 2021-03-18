/*
  ==============================================================================

    GenericControllableCommand.h
    Created: 28 Apr 2017 5:51:16pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

class GenericControllableCommand :
	public BaseCommand,
	public EngineListener
{
public:

	enum Action { SET_VALUE, TRIGGER};
	
	GenericControllableCommand(ChataigneGenericModule * _module, CommandContext context, var params, Multiplex* multiplex = nullptr);
	virtual ~GenericControllableCommand();
	
	enum Operator { EQUAL, INVERSE, ADD, SUBTRACT, MULTIPLY, DIVIDE, MAX, MIN };

	ChataigneGenericModule * sequenceModule;

	Action action;
	TargetParameter * target;
	EnumParameter* valueOperator;
	WeakReference<Parameter> value;
	var dataToLoad;
	var ghostValueData; // to keep when target is lost
	var ghostOperator;

	void updateValueFromTarget();
	void setValueParameter(Parameter * p);

	void updateOperatorOptions();

	virtual void triggerInternal(int multiplexIndex) override;

	void linkUpdated(ParameterLink* pLink) override;
	void onContainerParameterChanged(Parameter *) override;

	virtual void loadJSONDataInternal(var data) override;
	virtual void endLoadFile() override;

	static BaseCommand * create(ControllableContainer * module, CommandContext context, var params, Multiplex * multiplex = nullptr);

	//InspectableEditor * getEditor(bool isRoot) override;
};