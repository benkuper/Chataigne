/*
  ==============================================================================

    OSPowerCommand.cpp
    Created: 5 Jan 2018 4:05:45pm
    Author:  Ben

  ==============================================================================
*/

#include "OSPowerCommand.h"

OSPowerCommand::OSPowerCommand(OSModule * _module, CommandContext context, var params) :
	BaseCommand(_module, context, params)
{
	actionType = (ActionType)(int)params.getProperty("type", SHUTDOWN);
}

OSPowerCommand::~OSPowerCommand()
{
}

void OSPowerCommand::trigger()
{
	BaseCommand::trigger();

	switch (actionType)
	{
	case SHUTDOWN:
	{
#if JUCE_WINDOWS
		std::system("shutdown -s -t 0");
#elif JUCE_MAC
        std::system("osascript -e 'tell app \"System Events\" to shut down'"); 
#elif JUCE_LINUX
		int result = std::system("shutdown -h now");
		if(result != 0) LOGWARNING("Problem with shutdown command");
#endif
	}
		break;

	case REBOOT:
	{
#if JUCE_WINDOWS
		std::system("shutdown -r -t 0");
#elif JUCE_MAC
		std::system("osascript -e 'tell app \"System Events\" to restart'");
#elif JUCE_LINUX
		int result = std::system("shutdown -r now");
		if(result != 0) LOGWARNING("Problem with reboot command");
#endif
	}
		break;
	}
}
