/*
  ==============================================================================

    CustomVariablesModuleCommands.h
    Created: 23 Feb 2018 12:25:24am
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "Common/Command/BaseCommand.h"
#include "../CustomVariablesModule.h"

class CVCommand :
	public BaseCommand
{
public:
	CVCommand(CustomVariablesModule * _module, CommandContext context, var params);
	virtual ~CVCommand();

	CVGroupManager * manager;

	enum Type { SET_VALUE, SET_PRESET, LERP_PRESETS, SET_PRESET_WEIGHT, SET_2DTARGET };
	Type type;

	TargetParameter * target;
	TargetParameter * targetPreset;
	TargetParameter * targetPreset2;

	Parameter * value;

	void onContainerParameterChanged(Parameter * p) override;
	void trigger() override;

	static BaseCommand * create(ControllableContainer * module, CommandContext context, var params);
};