/*
  ==============================================================================

    OSWindowCommand.cpp
    Created: 8 Oct 2019 5:10:28pm
    Author:  bkupe

  ==============================================================================
*/

#include "OSWindowCommand.h"

#pragma warning(disable:4804)

OSWindowCommand::OSWindowCommand(OSModule* _module, CommandContext context, var params, Multiplex * multiplex) :
	BaseCommand(_module, context, params),
	Thread("OS WindowCommand"),
	focusFilter(nullptr),
	onTop(nullptr), decoration(nullptr), windowPos(nullptr), windowSize(nullptr)
{
	focusFilter = addEnumParameter("Filter", "How to filter windows with value");
	focusFilter->addOption("Contains", CONTAINS)->addOption("Starts With", STARTS_WITH)->addOption("Ends with", ENDS_WITH)->addOption("Exact match", EXACT_MATCH);
	
	target = addStringParameter("Name", "The name of the window, used with filter", "");
	
	onTop = addBoolParameter("Bring to top", "If checked, will bring to top, otherwise will leave where it is", false);
	
	decoration = addBoolParameter("Decoration", "Will enable or disable decoration. Disable this parameter to not change it", true, false);
	decoration->canBeDisabledByUser = true;
	
	windowPos = addPoint2DParameter("Position", "The window position. Leave disable to not change it", false);
	windowPos->canBeDisabledByUser = true;

	windowSize = addPoint2DParameter("Size", "The window size. Leave disable to not change it", false);
	windowSize->canBeDisabledByUser = true;
}

OSWindowCommand::~OSWindowCommand()
{
	signalThreadShouldExit();
	waitForThreadToExit(500);
}

void OSWindowCommand::triggerInternal(int multiplexIndex)
{
	if (isThreadRunning())
	{
		signalThreadShouldExit();
		waitForThreadToExit(500);
	}

	startThread();
	module->outActivityTrigger->trigger();
}

#if JUCE_WINDOWS
BOOL OSWindowCommand::enumWindowCallback(HWND hWnd, LPARAM lparam)
{
	int length = GetWindowTextLength(hWnd);
	char* buffer = new char[(int)length + 1];
	GetWindowText(hWnd, buffer, length + 1);
	std::string windowTitle(buffer);

	// Ignore windows if invisible or missing a title
	if (IsWindowVisible(hWnd) && length != 0) {
		String title = String(windowTitle);

		OSWindowCommand* c = reinterpret_cast<OSWindowCommand*>(lparam);
		FilterType t = c->focusFilter->getValueDataAsEnum<FilterType>();
		String target = c->target->stringValue();

		bool isValid = false;
		switch (t)
		{
		case CONTAINS: isValid = title.contains(target); break;
		case STARTS_WITH: isValid = title.startsWith(target); break;
		case ENDS_WITH: isValid = title.endsWith(target); break;
		case EXACT_MATCH: isValid = title == target; break;
		}

		if (isValid)
		{

			if (c->module->logOutgoingData->boolValue())
			{
				String s = "Set Window \"" + title + "\"parameters :";
				if (c->onTop->boolValue()) s += "\nBring to top";
				if (c->decoration->enabled) s += "\nDecoration " + String(c->decoration->boolValue() ? "enabled" : "disabled");
				if (c->windowPos->enabled) s += "\nPosition : " + String(c->windowPos->x) + ", " + String(c->windowPos->y);
				if (c->windowSize->enabled) s += "\nSize : " + String(c->windowSize->x) + ", " + String(c->windowSize->y);
				NLOG(c->module->niceName, s);
			}


			if (c->decoration->enabled)
			{
				LONG lStyle = GetWindowLong(hWnd, GWL_STYLE);
				LONG lExStyle = GetWindowLong(hWnd, GWL_EXSTYLE);

				LONG allStyle = (WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
				LONG allExStyle = (WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);

				lStyle = ~c->decoration->boolValue() ? lStyle | allStyle : lStyle & ~allStyle;
				lExStyle &= ~c->decoration->boolValue() ? lExStyle | allExStyle : lExStyle & ~allExStyle;

				SetWindowLong(hWnd, GWL_STYLE, c->decoration->boolValue() ? allStyle : WS_POPUP);
				//SetWindowLong(hWnd, GWL_STYLE, lStyle);
				//SetWindowLong(hWnd, GWL_EXSTYLE, lExStyle);
			}

			if (c->windowPos->enabled || c->windowSize || c->decoration)
			{
				UINT uFlags = SWP_SHOWWINDOW | SWP_NOREPOSITION | SWP_FRAMECHANGED | SWP_NOZORDER | (c->windowPos->enabled ? 0 : SWP_NOMOVE) | (c->windowSize->enabled ? 0 : SWP_NOSIZE);
				bool result = SetWindowPos(hWnd, HWND_NOTOPMOST, (int)c->windowPos->x, (int)c->windowPos->y, (int)c->windowSize->x, (int)c->windowSize->y, uFlags);
				if (!result) LOGWARNING("Could not set window " + title);
			}


			if (c->onTop->boolValue())
			{
				ShowWindow(hWnd, SW_RESTORE);
				SetForegroundWindow(hWnd);
			}


		}
	}

	return TRUE;
}
#endif


void OSWindowCommand::run()
{
#if JUCE_WINDOWS
	EnumWindows(enumWindowCallback, reinterpret_cast<LPARAM>(this));
#endif
}

