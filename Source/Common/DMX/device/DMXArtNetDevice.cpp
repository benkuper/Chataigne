/*
  ==============================================================================

    DMXArtNetDevice.cpp
    Created: 10 Apr 2017 12:44:42pm
    Author:  Ben

  ==============================================================================
*/

#include "DMXArtNetDevice.h"


DMXArtNetDevice::DMXArtNetDevice() :
	DMXDevice("ArtNet",ARTNET),
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
	DMXDevice::sendDMXValue(channel, value);
	fullMessage[channel + 18] = (uint8)value;
	fullMessage[12] = (uint8)sequence;
	
	sequence++;
	artNetOut.write(nodeIP->stringValue(), nodePort->intValue(), fullMessage, NUM_CHANNELS+18);
}
