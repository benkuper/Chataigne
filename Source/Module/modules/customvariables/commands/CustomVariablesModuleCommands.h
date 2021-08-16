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

	enum Type { SET_PRESET, GO_TO_PRESET, KILL_GO_TO_PRESET, LERP_PRESETS, SET_PRESET_WEIGHT, SET_2DTARGET, LOAD_PRESET, SAVE_PRESET, RANDOMIZE };
	Type type;

	TargetParameter * target;
	TargetParameter * targetPreset;
	TargetParameter * targetPreset2;
	FileParameter* presetFile;

	//interpolation
	FloatParameter* time;
	Automation* automation;
	
	Parameter* value;

	void onContainerParameterChanged(Parameter * p) override;
	void triggerInternal(int multiplexIndex) override;

	static BaseCommand * create(ControllableContainer * module, CommandContext context, var params, Multiplex * multiplex = nullptr);
};
