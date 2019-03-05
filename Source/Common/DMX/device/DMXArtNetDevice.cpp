/*
  ==============================================================================

    DMXArtNetDevice.cpp
    Created: 10 Apr 2017 12:44:42pm
    Author:  Ben

  ==============================================================================
*/

#include "DMXArtNetDevice.h"


DMXArtNetDevice::DMXArtNetDevice() :
	DMXDevice("ArtNet",ARTNET, true),
	sequence(0)
{
	nodeName = addStringParameter("Node Name", "Name of the art-net node", "Chataigne ArtNet");
	localPort = addIntParameter("Local Port", "Port of the local node, usually 6454", 6454, 1024, 65535);

	nodeIP = addStringParameter("Target Node IP", "IP of the ArtNet Device", "127.0.0.1");
	nodePort = addIntParameter("Target Node Port", "Port of the Node, usually 6454", 6454, 1024, 65535);
	
	memset(fullMessage, 0, (NUM_CHANNELS + 18) * sizeof(uint8));
	memcpy(fullMessage, header, 18 * sizeof(uint8));

	setupReceiver();
}

DMXArtNetDevice::~DMXArtNetDevice()
{
	artnet_destroy(node);
}

void DMXArtNetDevice::setupReceiver()
{

	node = artnet_new(nodeIP->stringValue().toRawUTF8(), true);
	
	artnet_set_short_name(node, StringUtil::toShortName(nodeName->stringValue()).toRawUTF8());
	artnet_set_long_name(node, nodeName->stringValue().toRawUTF8());
	artnet_set_node_type(node, ARTNET_RAW);

	artnet_set_port_type(node, 0, ARTNET_ENABLE_INPUT, ARTNET_PORT_DMX);
	artnet_set_port_addr(node, 0, ARTNET_INPUT_PORT, 0 /* universe */);
	
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
    
	//fullMessage[12] = (uint8)sequence;

	//sequence++;
	//artNetOut.write(nodeIP->stringValue(), nodePort->intValue(), fullMessage, NUM_CHANNELS + 18);

	artnet_send_dmx(node,0  /* universe */, 512, dmxDataOut);
}
