/*
  ==============================================================================

    WakeOnLanCommand.cpp
    Created: 31 Mar 2018 11:06:18am
    Author:  Ben

  ==============================================================================
*/

#include "WakeOnLanCommand.h"
#include "ui/WakeOnLanCommandEditor.h"

WakeOnLanCommand::WakeOnLanCommand(OSModule * _module, CommandContext context, var params) :
	BaseCommand(_module, context, params),
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

void WakeOnLanCommand::triggerInternal()
{
	if (!module->enabled->boolValue()) return;

	if (mac.isNull())
	{
		LOG("WOL : mac is empty, not sending");
		return;
	}

	uint8 ffBytes[6] = { 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF };
	const uint8 *macBytes = mac.getBytes();

	const int packetSize = 6 + 16 * 6;
	uint8 packet[packetSize];

	for (int i = 0; i < 6; ++i) packet[i] = ffBytes[i];

	for (int i = 0; i < 16; ++i)
	{
		for (int j = 0; j < 6; j++)
		{
			int index = 6 + i * 6 + j;
			packet[index] = macBytes[j];
		}
	}

	DatagramSocket s(true);
	s.bindToPort(0);
	s.write(IPAddress::broadcast().toString(), 9, packet, packetSize);

	osModule->outActivityTrigger->trigger();
}

InspectableEditor * WakeOnLanCommand::getEditor(bool isRoot)
{
	return new WakeOnLanCommandEditor(this, isRoot);
}
