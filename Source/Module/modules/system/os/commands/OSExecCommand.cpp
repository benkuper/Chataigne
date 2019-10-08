/*
  ==============================================================================

    OSFileCommand.cpp
    Created: 5 Jan 2018 4:05:49pm
    Author:  Ben

  ==============================================================================
*/

#include "OSExecCommand.h"

#if JUCE_WINDOWS
#include <windows.h>
#include <Tlhelp32.h>
#endif

OSExecCommand::OSExecCommand(OSModule* _module, CommandContext context, var params) :
	BaseCommand(_module, context, params),
	launchOptions(nullptr),
	silentMode(nullptr),
	focusFilter(nullptr),
	onTop(nullptr), decoration(nullptr), windowPos(nullptr), windowSize(nullptr)
{
	actionType = (ActionType)(int)params.getProperty("type", LAUNCH_APP);

	switch (actionType)
	{
	case OPEN_FILE:
	case LAUNCH_APP:
	case LAUNCH_COMMAND_FILE:
		target = new FileParameter("Target", "The target file of app for this command", "");
		addParameter(target);
		if (actionType != LAUNCH_COMMAND_FILE)
		{
			launchOptions = addStringParameter("Launch Options", "Additional options when launching the app", "");
			launchOptions->multiline = true;
		}
		break;

	case KILL_APP:
		target = addStringParameter("Target", "The process name to kill", "");
		killMode = addBoolParameter("Hard kill", "If enabled, will kill like a boss, not very gently", false);
		break;

	case LAUNCH_COMMAND:
		target = addStringParameter("Command", "The command to launch.Every line is a new command", "");
		target->multiline = true;
		break;

	case SET_WINDOW:
		focusFilter = addEnumParameter("Filter", "How to filter windows with value");
		focusFilter->addOption("Contains", CONTAINS)->addOption("Starts With", STARTS_WITH)->addOption("Ends with", ENDS_WITH)->addOption("Exact match", EXACT_MATCH);
		target = addStringParameter("Name", "The name of the window, used with filter", "");
		onTop = addBoolParameter("Bring to top", "If checked, will bring to top, otherwise will leave where it is", false);
		decoration = addBoolParameter("Decoration", "Will enable or disable decoration. Disable this parameter to not change it", true, false);
		decoration->canBeDisabledByUser = true;
		windowPos = addPoint2DParameter("Position", "The window position. Leave disable to not change it", false);
		windowPos->canBeDisabledByUser = true;
		windowSize = addPoint2DParameter("Position", "The window size. Leave disable to not change it", false);
		windowSize->canBeDisabledByUser = true;
		break;
	}

	if (actionType == LAUNCH_COMMAND || actionType == LAUNCH_COMMAND_FILE)
	{
		silentMode = addBoolParameter("Silent mode", "If checked, will not open a new terminal window, but if the process never stops, there is no way to shut it down without closing Chataigne !", false);
	}
}

OSExecCommand::~OSExecCommand()
{
}

void OSExecCommand::triggerInternal()
{
	BaseCommand::triggerInternal();

	if (!module->enabled->boolValue()) return;

	switch (actionType)
	{
	case LAUNCH_APP:
	case OPEN_FILE:
	{
		File f = ((FileParameter*)target)->getFile();
		File wDir = File::getCurrentWorkingDirectory();

		f.getParentDirectory().setAsCurrentWorkingDirectory();
		bool result = f.startAsProcess(launchOptions->stringValue());
		if (!result) NLOGERROR(module->niceName, "Error trying to launch process : " << f.getFullPathName());
		wDir.setAsCurrentWorkingDirectory();
		module->outActivityTrigger->trigger();
	}
	break;

	case KILL_APP:
	{
		killProcess(target->stringValue());
		module->outActivityTrigger->trigger();
	}
	break;

	case LAUNCH_COMMAND:
	{
		String command = target->stringValue().replace("\n", " && ");
		int result = 0;
#if JUCE_WINDOWS
        if(silentMode->boolValue()) WinExec(command.getCharPointer(), SW_HIDE);
		else result = system(command.getCharPointer());
#else
        result = system(command.getCharPointer());
#endif
		if (module->logOutgoingData->boolValue()) NLOG(module->niceName, "Launching : " + command);
		if (result != 0) NLOGERROR(module->niceName, "Error trying to launch command : " << target->stringValue());
		module->outActivityTrigger->trigger();
	}
	break;

	case LAUNCH_COMMAND_FILE:
	{
		File f = ((FileParameter*)target)->getFile();
		String dir = f.getParentDirectory().getFullPathName();
		
#if JUCE_WINDOWS
		String driveLetter = dir.substring(0, 2);
		String command = driveLetter + " && cd \"" + dir + "\" && \"" + f.getFileName()+"\"";
		if (module->logOutgoingData->boolValue()) NLOG(module->niceName, "Launching : " + command);
		int result = 0;
		if (silentMode->boolValue()) WinExec(command.getCharPointer(), SW_HIDE);
		else system(command.getCharPointer());
#else
        String launchPrefix = f.getFileName().endsWith("sh")?"sh ":"./";
        
    #if JUCE_MAC
        
        String command = "osascript -e 'tell application \"Terminal\""
        +String("\nactivate")
        +"\ndo script \"cd "+ dir +" && "+launchPrefix + "\\\""+f.getFileName()+"\\\"\""
        +"\nend tell'";
        
            //"osascript -e 'tell application \"Terminal\" to do script \"cd "+ dir +" && "+launchPrefix + f.getFileName()+"\"'";
    #else //linux
        String command = "cd \"" + dir + "\" && gnome-terminal -- bash -c '" + launchPrefix + "\""+ f.getFileName()+"\"'";
    #endif
        
        if (module->logOutgoingData->boolValue()) NLOG(module->niceName, "Launching : " + command);
        int result = system(command.getCharPointer());
#endif
        
		if (result != 0) NLOGERROR(module->niceName, "Error trying to launch command : " << f.getFullPathName());
		module->outActivityTrigger->trigger();
	}
	break;

	case SET_WINDOW:
#if JUCE_WINDOWS
		EnumWindows(enumWindowCallback, reinterpret_cast<LPARAM>(this));
#endif
		module->outActivityTrigger->trigger();
		break;
	}
}

void OSExecCommand::killProcess(const String & name)
{
#if JUCE_WINDOWS
	int result = system(String("taskkill " + String(killMode->boolValue() ? "/f " : "") + "/im \"" + target->stringValue() + "\"").getCharPointer());
	if (result != 0) LOGWARNING("Problem killing app " + target->stringValue());
#else
	int result = system(String("killall "+ String(killMode->boolValue()?"-9":"-2") +" \""+target->stringValue()+"\"").getCharPointer());
	if(result != 0) LOGWARNING("Problem killing app " + target->stringValue());
#endif
}

#if JUCE_WINDOWS
BOOL OSExecCommand::enumWindowCallback(HWND hWnd, LPARAM lparam) 
{
	int length = GetWindowTextLength(hWnd);
	char* buffer = new char[length + 1];
	GetWindowText(hWnd, buffer, length + 1);
	std::string windowTitle(buffer);

	// Ignore windows if invisible or missing a title
	if (IsWindowVisible(hWnd) && length != 0) {
		String title = String(windowTitle);

		OSExecCommand* c = reinterpret_cast<OSExecCommand*>(lparam);
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
				if (c->decoration->enabled) s += "\nDecoration " + String(c->decoration->boolValue()?"enabled":"disabled");
				if (c->windowPos->enabled) s += "\nPosition : " + String(c->windowPos->x) + ", " + String(c->windowPos->y);
				if (c->windowSize->enabled) s += "\nSize : "+String(c->windowSize->x)+", "+String(c->windowSize->y);
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

				SetWindowLong(hWnd /*The handle of the window to remove its borders*/, GWL_STYLE, c->decoration->boolValue()?allStyle:WS_POPUP);
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
