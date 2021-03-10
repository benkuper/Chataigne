/*
  ==============================================================================

    CustomVariablesModuleCommands.h
    Created: 23 Feb 2018 12:25:24am
    Author:  Ben

  ==============================================================================
*/

#pragma once

class CustomVariablesModule;
class CVGroupManager;

class CVCommand :
	public BaseCommand
{
public:
	CVCommand(CustomVariablesModule * _module, CommandContext context, var params, Multiplex* multiplex = nullptr);
	virtual ~CVCommand();

	CVGroupManager * manager;

	enum Type { SET_VALUE, SET_PRESET, GO_TO_PRESET, KILL_GO_TO_PRESET, LERP_PRESETS, SET_PRESET_WEIGHT, SET_2DTARGET, LOAD_PRESET, SAVE_PRESET };
	Type type;

	TargetParameter * target;
	TargetParameter * targetPreset;
	TargetParameter * targetPreset2;
	FileParameter* presetFile;

	//interpolation
	FloatParameter* time;
	Automation* automation;
	
	enum Operator { EQUAL, INVERSE, ADD, SUBTRACT, MULTIPLY, DIVIDE, MAX, MIN };
	EnumParameter * valueOperator; 
	Parameter * value;
	var ghostValueData;

	void updateValueFromTarget();
	void updateOperatorOptions();

	void onContainerParameterChanged(Parameter * p) override;
	void triggerInternal(int multiplexIndex) override;

	void linkUpdated(ParameterLink* pLink) override;

	static BaseCommand * create(ControllableContainer * module, CommandContext context, var params, Multiplex * multiplex = nullptr);
};
