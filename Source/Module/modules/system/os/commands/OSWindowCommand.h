/*
  ==============================================================================

	OSWindowCommand.h
	Created: 5 Jan 2018 4:05:49pm
	Author:  Ben

  ==============================================================================
*/

#pragma once

#if JUCE_WINDOWS
#include <Windows.h>
#endif

class OSWindowCommand :
	public BaseCommand,
	public Thread
{
public:
	OSWindowCommand(OSModule* _module, CommandContext context, var params, Multiplex * multiplex = nullptr);
	~OSWindowCommand();

	enum FilterType { CONTAINS, STARTS_WITH, ENDS_WITH, EXACT_MATCH };
	EnumParameter* focusFilter;
	StringParameter* target;

	BoolParameter* onTop;
	
	BoolParameter* decoration;
	Point2DParameter* windowPos;
	Point2DParameter* windowSize;

	void triggerInternal(int multiplexIndex) override;

#if JUCE_WINDOWS
	static BOOL CALLBACK enumWindowCallback(HWND hWnd, LPARAM lparam);
#endif

	void run() override;

	static OSWindowCommand* create(ControllableContainer* module, CommandContext context, var params, Multiplex * multiplex) { return new OSWindowCommand((OSModule*)module, context, params, multiplex); }
};
