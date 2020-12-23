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

#if JUCE_WINDOWS
#include <Windows.h>
#endif

class OSExecCommand :
	public BaseCommand
{
public:
	OSExecCommand(OSModule * _module, CommandContext context, var params, IteratorProcessor * iterator = nullptr);
	~OSExecCommand();

	enum ActionType { OPEN_FILE, LAUNCH_APP, KILL_APP, LAUNCH_COMMAND, LAUNCH_COMMAND_FILE };
	ActionType actionType;

	OSModule* osModule;

	StringParameter * target;
	StringParameter * launchOptions;
	BoolParameter * killMode;
	BoolParameter* silentMode;

	void triggerInternal() override;

	void killProcess(const String &name);

	static OSExecCommand * create(ControllableContainer * module, CommandContext context, var params, IteratorProcessor * iterator) { return new OSExecCommand((OSModule *)module, context, params, iterator); }

};
