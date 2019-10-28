/*
  ==============================================================================

    ReaperTimeCommand.h
    Created: 26 Jan 2017 2:42:39pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../../commands/OSCCommand.h"
#include "../ReaperModule.h"

class ReaperTimeCommand :
	public OSCCommand
{
public:
	ReaperTimeCommand(ReaperModule* _module, CommandContext context, var params);
	~ReaperTimeCommand();

	ReaperModule* reaperModule;
	BoolParameter* stopTimePlay;
	FloatParameter* timeParam;

	void triggerInternal() override;

	static ReaperTimeCommand* create(ControllableContainer* module, CommandContext context, var params) { return new ReaperTimeCommand((ReaperModule*)module, context, params); }

};