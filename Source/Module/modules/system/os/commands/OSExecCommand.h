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
	OSExecCommand(OSModule * _module, CommandContext context, var params);
	~OSExecCommand();

	enum ActionType { OPEN_FILE, LAUNCH_APP, KILL_APP, LAUNCH_COMMAND, LAUNCH_COMMAND_FILE, SET_WINDOW };
	ActionType actionType;

	enum FilterType { CONTAINS, STARTS_WITH, ENDS_WITH, EXACT_MATCH };
	EnumParameter* focusFilter;

	StringParameter * target;
	StringParameter * launchOptions;
	BoolParameter * killMode;
	BoolParameter* silentMode;
	
	BoolParameter* onTop;
	BoolParameter* decoration;
	Point2DParameter* windowPos;
	Point2DParameter* windowSize;

	void triggerInternal() override;

	void killProcess(const String &name);

#if JUCE_WINDOWS
	static BOOL CALLBACK enumWindowCallback(HWND hWnd, LPARAM lparam);
#endif

	static OSExecCommand * create(ControllableContainer * module, CommandContext context, var params) { return new OSExecCommand((OSModule *)module, context, params); }

};
