/*
  ==============================================================================

    GenericOSCQueryCommand.h
    Created: 28 Feb 2019 10:33:50pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class GenericOSCQueryCommand :
	public BaseCommand,
	public EngineListener
{
public:
	
	enum Action { SET_VALUE, TRIGGER };
	
	GenericOSCQueryCommand(GenericOSCQueryModule * module, CommandContext context, var params, Multiplex * multiplex = nullptr);
	~GenericOSCQueryCommand();

	GenericOSCQueryModule * oscQueryModule;
	enum Operator { EQUAL, INVERSE, ADD, SUBTRACT, MULTIPLY, DIVIDE, MAX, MIN };

	Action action;
	TargetParameter* target;
	EnumParameter* valueOperator;
	WeakReference<Parameter> value;
	var dataToLoad;
	var ghostValueData; // to keep when target is lost
	var ghostOperator;

	void updateValueFromTarget();
	void setValueParameter(Parameter* p);

	void updateOperatorOptions();

	virtual void triggerInternal(int multiplexIndex) override;

	void linkUpdated(ParameterLink* pLink) override;
	void onContainerParameterChanged(Parameter*) override;

	virtual void loadJSONDataInternal(var data) override;
	virtual void endLoadFile() override;

	virtual void loadGhostData(var data);

	static GenericOSCQueryCommand * create(ControllableContainer * module, CommandContext context, var params, Multiplex * multiplex) { 
		return new GenericOSCQueryCommand((GenericOSCQueryModule *)module, context, params, multiplex); }
};