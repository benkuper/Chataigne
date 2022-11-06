/*
  ==============================================================================

	OSPowerCommand.cpp
	Created: 5 Jan 2018 4:05:45pm
	Author:  Ben

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

OSPowerCommand::OSPowerCommand(OSModule* _module, CommandContext context, var params, Multiplex* multiplex) :
	BaseCommand(_module, context, params, multiplex)
{
	actionType = (ActionType)(int)params.getProperty("type", SHUTDOWN);
	force = addBoolParameter("Force", "Force action without notifying user if applications are stopping the proccedure", false);
}

OSPowerCommand::~OSPowerCommand()
{
}

void OSPowerCommand::triggerInternal(int multiplexIndex)
{
	BaseCommand::triggerInternal(multiplexIndex);

	if (!module->enabled->boolValue()) return;

	switch (actionType)
	{
	case SHUTDOWN:
	{
#if JUCE_WINDOWS
		String s = "shutdown -s -t 0";
		if (force->boolValue()) s += " -f";
		std::system(s.toStdString().c_str());
#elif JUCE_MAC
		std::system("osascript -e 'tell app \"System Events\" to shut down'");
#elif JUCE_LINUX
		int result = std::system("shutdown -h now");
		if (result != 0) LOGWARNING("Problem with shutdown command");
#endif
	}
	break;

	case REBOOT:
	{
#if JUCE_WINDOWS
		String s = "shutdown -r -t 0";
		if (force->boolValue()) s += " -f";
		std::system(s.toStdString().c_str());
#elif JUCE_MAC
		std::system("osascript -e 'tell app \"System Events\" to restart'");
#elif JUCE_LINUX
		int result = std::system("shutdown -r now");
		if (result != 0) LOGWARNING("Problem with reboot command");
#endif
	}
	break;
	}
}
