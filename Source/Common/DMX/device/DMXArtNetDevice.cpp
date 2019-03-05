/*
  ==============================================================================

    DMXArtNetDevice.cpp
    Created: 10 Apr 2017 12:44:42pm
    Author:  Ben

  ==============================================================================
*/

#include "DMXArtNetDevice.h"


DMXArtNetDevice::DMXArtNetDevice() :
	DMXDevice("ArtNet",ARTNET, false),
	sequence(0)
{
	nodeIP = addStringParameter("Node IP", "IP of the ArtNet Device", "192.168.1.255");
	nodePort = addIntParameter("Node Port", "Port of the Node, usually 6454", 6454, 1024, 65535);
	
	memset(fullMessage, 0, (NUM_CHANNELS + 18) * sizeof(uint8));
	memcpy(fullMessage, header, 18 * sizeof(uint8));
}

DMXArtNetDevice::~DMXArtNetDevice()
{
}

void DMXArtNetDevice::sendDMXValue(int channel, int value)
{
	fullMessage[channel-1 + 18] = (uint8)value;
	DMXDevice::sendDMXValue(channel, value);
}

void DMXArtNetDevice::sendDMXRange(int startChannel, Array<int> values)
{
	int numValues = values.size();
	for (int i = 0; i < numValues; i++)
	{
		int channel = startChannel + i;
		if (channel < 0) continue;
		if (channel > 512) break;

		fullMessage[channel - 1 + 18] = (uint8)(values[i]);
	}

	DMXDevice::sendDMXRange(startChannel, values);

}

void DMXArtNetDevice::sendDMXValues()
{
    if(nodeIP == nullptr || nodePort == nullptr) return;
    
	fullMessage[12] = (uint8)sequence;

	sequence++;
	artNetOut.write(nodeIP->stringValue(), nodePort->intValue(), fullMessage, NUM_CHANNELS + 18);
}
