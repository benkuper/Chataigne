/*
  ==============================================================================

    OSFileCommand.h
    Created: 5 Jan 2018 4:05:49pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "Common/Command/BaseCommand.h"
#include "../OSModule.h"

class OSExecCommand :
	public BaseCommand
{
public:
	OSExecCommand(OSModule * _module, CommandContext context, var params);
	~OSExecCommand();

	enum ActionType { OPEN_FILE, LAUNCH_APP, KILL_APP };
	ActionType actionType;

	StringParameter * target;
	StringParameter * launchOptions;
	BoolParameter * killMode;

	void triggerInternal() override;

	void killProcess(const String &name);

	static OSExecCommand * create(ControllableContainer * module, CommandContext context, var params) { return new OSExecCommand((OSModule *)module, context, params); }

};
