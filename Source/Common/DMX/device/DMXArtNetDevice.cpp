/*
  ==============================================================================

	DMXArtNetDevice.cpp
	Created: 10 Apr 2017 12:44:42pm
	Author:  Ben

  ==============================================================================
*/

#include "DMXArtNetDevice.h"

#if JUCE_WINDOWS
#include <iphlpapi.h>
#endif

DMXArtNetDevice::DMXArtNetDevice() :
	DMXDevice("ArtNet", ARTNET, true),
    nodeName(nullptr),
    ioNode(nullptr),
	discoverNode(nullptr),
	numFoundNodes(0),
    noServerCreation(false)
{
	networkInterface = addEnumParameter("Interface", "Interface to link the node to");
	
	Array<NetworkInterface> interfaces = getAllInterfaces();
	networkInterface->addOption("All (Default)", "")->addOption("Broadcast (255.255.255.255)", "255.255.255.255");
	for (auto &i : interfaces)
	{
		networkInterface->addOption(i.interfaceName + " (" + i.ipAddress + ")", i.ipAddress, true);
	}
	
	nodeName = addStringParameter("Node Name", "Name of the art-net node", "Chataigne ArtNet");

	if (Engine::mainEngine->isLoadingFile) Engine::mainEngine->addEngineListener(this);
	else setupReceiver();
}

DMXArtNetDevice::~DMXArtNetDevice()
{
	if (Engine::mainEngine != nullptr) Engine::mainEngine->removeEngineListener(this);
	if(ioNode != nullptr) artnet_destroy(ioNode);
	if(discoverNode != nullptr) artnet_destroy(discoverNode);
}

void DMXArtNetDevice::setupReceiver()
{
	if (nodeName == nullptr) return;
	if (noServerCreation) return;
	if (Engine::mainEngine != nullptr && (Engine::mainEngine->isLoadingFile || Engine::mainEngine->isClearing)) return;

	setConnected(false);

	if (ioNode != nullptr) artnet_destroy(ioNode);

	ioNode = artnet_new(networkInterface->getValue().toString().isNotEmpty()? networkInterface->getValue().toString().toRawUTF8():NULL, false);// , true); ;

	if (ioNode == nullptr)
	{
		LOGWARNING("Setup Artnet Node failed, please try with another network interface");
		return;
	}

	artnet_set_short_name(ioNode, StringUtil::toShortName(nodeName->stringValue()).toRawUTF8());
	artnet_set_long_name(ioNode, nodeName->stringValue().toRawUTF8());
	artnet_set_node_type(ioNode, artnet_node_type::ARTNET_NODE);

	artnet_set_port_type(ioNode, 0, ARTNET_ENABLE_INPUT, ARTNET_PORT_DMX);
	artnet_set_port_addr(ioNode, 0, ARTNET_INPUT_PORT, 0 /* universe */);
	//artnet_set_subnet_addr(ioNode, 0);

	artnet_set_port_type(ioNode, 1, ARTNET_ENABLE_OUTPUT, ARTNET_PORT_DMX);
	artnet_set_port_addr(ioNode, 1, ARTNET_OUTPUT_PORT, 0);

	artnet_set_dmx_handler(ioNode, &DMXArtNetDevice::artNetDMXReceiveHandler, this);

	if (artnet_start(ioNode) != ARTNET_EOK)
	{
		LOGWARNING("Could not start " << artnet_strerror());
	}

	LOG("ArtNet Node created with name " << nodeName->stringValue());

	//ioNode = artnet_new(remoteHost->stringValue().toRawUTF8(), true);

	//artnet_set_short_name(ioNode, StringUtil::toShortName(nodeName->stringValue()).toRawUTF8());
	//artnet_set_long_name(discoverNode, nodeName->stringValue().toRawUTF8());
	//artnet_set_node_type(discoverNode, ARTNET_RAW);

	setConnected(true);
	
}

void DMXArtNetDevice::setupNode()
{
	if (discoverNode != nullptr) artnet_destroy(discoverNode);

	discoverNode = artnet_new(networkInterface->getValue().toString().isNotEmpty() ? networkInterface->getValue().toString().toRawUTF8() : NULL, true);
	
	if (discoverNode == nullptr)
	{
		//LOGWARNING("Node setup failed");
		return;
	}

	artnet_set_short_name(discoverNode, StringUtil::toShortName(nodeName->stringValue()).toRawUTF8());
	artnet_set_long_name(discoverNode, nodeName->stringValue().toRawUTF8());
	artnet_set_node_type(discoverNode, ARTNET_SRV);
	artnet_set_handler(discoverNode, ARTNET_REPLY_HANDLER, &DMXArtNetDevice::artNetReplyHandler, this);
	
	numFoundNodes = 0;
}

void DMXArtNetDevice::discoverNodes()
{
	if (discoverNode == nullptr)
	{
		//DBG("output node null");
		return;
	}

	if (artnet_start(discoverNode) != ARTNET_EOK) {
		//LOGWARNING("Failed to start: " << artnet_strerror());
		return;
	}

	artnet_get_sd(discoverNode);

	if (artnet_send_poll(discoverNode, NULL, ARTNET_TTM_DEFAULT) != ARTNET_EOK) {
		//LOGWARNING("send poll failed\n");
		return;
	}
}

void DMXArtNetDevice::sendDMXValue(int channel, int value)
{
	//	fullMessage[channel-1 + 18] = (uint8)value;
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

		//		fullMessage[channel - 1 + 18] = (uint8)(values[i]);
	}

	DMXDevice::sendDMXRange(startChannel, values);

}

void DMXArtNetDevice::sendDMXValues()
{
	//  if(nodeIP == nullptr || nodePort == nullptr) return;

	  //fullMessage[12] = (uint8)sequence;

	  //sequence++;
	  //artNetOut.write(nodeIP->stringValue(), nodePort->intValue(), fullMessage, NUM_CHANNELS + 18);

	artnet_send_dmx(ioNode, 0  /* universe */, 512, dmxDataOut);
}

int DMXArtNetDevice::artNetReplyHandler(artnet_node node, void * pp, void * devicePtr)
{
	DMXArtNetDevice * device = (DMXArtNetDevice *)devicePtr;
	if (device == nullptr)
	{
		LOGWARNING("DEVICE NULL");
		return 0;
	}

	artnet_node_list nl = artnet_get_nl(node);
	if (device->numFoundNodes == artnet_nl_get_length(nl)) {
		// this is not a new node, just a previously discovered one sending
		// another reply
		LOG("HERE FOUND 0");
		return 0;
	}
	else
	{
		artnet_node_entry ne = artnet_nl_first(nl);
		LOG(String::formatted("--------- %d.%d.%d.%d -------------\n", ne->ip[0], ne->ip[1], ne->ip[2], ne->ip[3]));
		LOG(String::formatted("Short Name:   %s\n", ne->shortname));
		LOG(String::formatted("Long Name:    %s\n", ne->longname));
		LOG(String::formatted("Node Report:  %s\n", ne->nodereport));
		LOG(String::formatted("Subnet:       0x%02x\n", ne->sub));
		LOG(String::formatted("Numb Ports:   %d\n", ne->numbports));
		LOG(String::formatted("Input Addrs:  0x%02x, 0x%02x, 0x%02x, 0x%02x\n", ne->swin[0], ne->swin[1], ne->swin[2], ne->swin[3]));
		LOG(String::formatted("Output Addrs: 0x%02x, 0x%02x, 0x%02x, 0x%02x\n", ne->swout[0], ne->swout[1], ne->swout[2], ne->swout[3]));
		LOG(String::formatted("----------------------------------\n"));
	}
	
	return 0;
}

int DMXArtNetDevice::artNetDMXReceiveHandler(artnet_node node, int port, void * devicePtr)
{
	DMXArtNetDevice * device = (DMXArtNetDevice *)devicePtr;
	if (device == nullptr)
	{
		LOGWARNING("DEVICE NULL");
		return 0;
	}
	int length = 0;
	uint8_t *dmx = artnet_read_dmx(device->ioNode, port, &length);
	
	for (int i = 0; i < length; i++)
	{
		device->setDMXValueIn(i + 1, dmx[i]);
	}

	return 0;
}

void DMXArtNetDevice::endLoadFile()
{
	Engine::mainEngine->removeEngineListener(this); 
	setupReceiver();
}


Array<DMXArtNetDevice::NetworkInterface> DMXArtNetDevice::getAllInterfaces()
{
	Array<NetworkInterface> result;

#if JUCE_WINDOWS
	PIP_ADAPTER_ADDRESSES pAddresses = NULL;
	ULONG outBufLen = 15e3;
	pAddresses = (IP_ADAPTER_ADDRESSES *)malloc(outBufLen);

	DWORD addResult = addResult = GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX, NULL, pAddresses, &outBufLen);
	
	if (addResult == NO_ERROR) {
		
		PIP_ADAPTER_ADDRESSES a = pAddresses;

		while (a) {
			char * ipData = a->FirstUnicastAddress->Address.lpSockaddr->sa_data;
			String ip = String((uint8)ipData[2]) + "." + String((uint8)ipData[3]) + "." + String((uint8)ipData[4]) + "." + String((uint8)ipData[5]);
			result.add({ a->FriendlyName, ip });
			a = a->Next;
		}
	}
	else
	{
		LOGWARNING("Failed retrieving adapters addresses : " << addResult);
	}

	if (pAddresses)  free(pAddresses);
#endif

	return result;
}

void DMXArtNetDevice::onContainerParameterChanged(Parameter * p)
{
	DMXDevice::onContainerParameterChanged(p);
	if (p == networkInterface || p == nodeName)
	{
		setupReceiver();
		setupNode();
		discoverNodes();
	}
}

void DMXArtNetDevice::onControllableFeedbackUpdate(ControllableContainer * cc, Controllable * c)
{
	DMXDevice::onControllableFeedbackUpdate(cc, c);
}


void DMXArtNetDevice::runInternal()
{
	if (threadShouldExit()) return;

	if (ioNode != nullptr)
	{
		artnet_read(ioNode, 1);
	}
}
