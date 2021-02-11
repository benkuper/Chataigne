/*
  ==============================================================================

    WakeOnLanCommand.cpp
    Created: 31 Mar 2018 11:06:18am
    Author:  Ben

  ==============================================================================
*/

#include "WakeOnLanCommand.h"
#include "ui/WakeOnLanCommandEditor.h"

WakeOnLanCommand::WakeOnLanCommand(OSModule * _module, CommandContext context, var params, Multiplex * multiplex) :
	BaseCommand(_module, context, params, multiplex),
	osModule(_module)
{
	macAddress = addStringParameter("MAC Address", "Mac address in the format FF:FF:FF:FF:FF:FF", "00:00:00:00:00:00");
	generateMacFromString();

	hideEditorHeader = false;
}

WakeOnLanCommand::~WakeOnLanCommand()
{

}

void WakeOnLanCommand::generateMacFromString()
{
	if (macAddress->stringValue().length() == 17)
	{
		unsigned char macBytes[6];
		sscanf(macAddress->stringValue().getCharPointer(), "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &macBytes[0], &macBytes[1], &macBytes[2], &macBytes[3], &macBytes[4], &macBytes[5]);
		mac = MACAddress(macBytes);
	} else
	{
		LOG("MAC Address is not in the right format. It should look like FF:FF:FF:FF:FF:FF");
	}
}

void WakeOnLanCommand::onContainerParameterChanged(Parameter * p)
{
	if (p == macAddress)
	{
		generateMacFromString();
	}
}

void WakeOnLanCommand::triggerInternal(int multiplexIndex)
{
	if (!module->enabled->boolValue()) return;

	if (mac.isNull())
	{
		LOG("WOL : mac is empty, not sending");
		return;
	}

	int result = WakeOnLan::wake(&mac);

	if (result != -1)
	{
		if (module->logOutgoingData->boolValue()) LOG("Wake On Lan sent to " << mac.toString());
	}
	else
	{
		LOGERROR("Error sending Wake On Lan to " + mac.toString());
	}

	osModule->outActivityTrigger->trigger();
}

InspectableEditor * WakeOnLanCommand::getEditor(bool isRoot)
{
	return new WakeOnLanCommandEditor(this, isRoot);
}
