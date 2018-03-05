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

OSExecCommand::OSExecCommand(OSModule * _module, CommandContext context, var params) :
	BaseCommand(_module, context, params)
{
	actionType = (ActionType)(int)params.getProperty("type", LAUNCH_APP);

	

	if (actionType != KILL_APP)
	{
		target = new FileParameter("Target", "The target file of app for this command", "");
		addParameter(target);
		launchOptions = addStringParameter("Launch Options", "Additional options when launching the app", "");
	} else
	{
		target = addStringParameter("Target", "The process name to kill", "");
#if JUCE_MAC || JUCE_LINUX
		killMode = addBoolParameter("Hard kill", "If enabled, will kill like a boss, not very gently", false);
#endif
	}
}

OSExecCommand::~OSExecCommand()
{
}

void OSExecCommand::trigger()
{
	BaseCommand::trigger();

	bool result = false;
	switch (actionType)
	{
	case LAUNCH_APP:
	case OPEN_FILE:
	{
		File f = ((FileParameter *)target)->getFile();
		File wDir = File::getCurrentWorkingDirectory();

		f.getParentDirectory().setAsCurrentWorkingDirectory();
		result = f.startAsProcess(launchOptions->stringValue());
		wDir.setAsCurrentWorkingDirectory();
	}
		break;

	case KILL_APP:
		killProcess(target->stringValue());
		break;
	}
}

void OSExecCommand::killProcess(const String & name)
{
#if JUCE_WINDOWS
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof(pEntry);
	BOOL hRes = Process32First(hSnapShot, &pEntry);
	while (hRes)
	{
		if (strcmp(pEntry.szExeFile, name.getCharPointer()) == 0)
		{
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0,
				(DWORD)pEntry.th32ProcessID);
			if (hProcess != NULL)
			{
				TerminateProcess(hProcess, 9);
				CloseHandle(hProcess);
			}
		}
		hRes = Process32Next(hSnapShot, &pEntry);
	}
	CloseHandle(hSnapShot);
#else
	int result = system(String("killall "+ String(killMode->boolValue()?"-9":"-2") +" \""+target->stringValue()+"\"").getCharPointer());
	if(result != 0) LOGWARNING("Problem killing app " + target->stringValue());
#endif
}
