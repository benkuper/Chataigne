/*
  ==============================================================================

    DMXArtNetDevice.h
    Created: 10 Apr 2017 12:44:42pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "DMXDevice.h"
//#include "artnet.h"

#define OP_CODE 0x5000
#define PROTOCOL_VERSION 14
#define NUM_CHANNELS 512

class DMXArtNetDevice :
	public DMXDevice,
	public EngineListener,
	public Thread //receiving
{
public:
	DMXArtNetDevice();
	~DMXArtNetDevice();

	//EnumParameter * networkInterface;
	IntParameter* subnet;
	IntParameter* universe;
	//StringParameter * nodeName;

	IntParameter * localPort;
	StringParameter* remoteHost;
	IntParameter * remotePort;

	DatagramSocket receiver;
	DatagramSocket sender;

	uint8 sequenceNumber;
	char artnetPacket[530]{ 'A','r','t','-','N','e','t',0, 0x00, 0x50,  0, 14 };

	//artnet_node ioNode;
	//artnet_node discoverNode;

	//int numFoundNodes;
	//bool noServerCreation;


	void setupReceiver();
	void setupNode();
	void discoverNodes();

	void sendDMXValue(int channel, int value) override;
	void sendDMXRange(int startChannel, Array<int> values) override;

	void sendDMXValues() override;

	//static int artNetReplyHandler(artnet_node node, void * pp, void * devicePtr);
	//static int artNetDMXReceiveHandler(artnet_node node, int port, void * data);


	void endLoadFile() override;

	struct NetworkInterface
	{
		String interfaceName;
		String ipAddress;
	};

	//Array<NetworkInterface> getAllInterfaces();

	void onContainerParameterChanged(Parameter * p) override;
	void onControllableFeedbackUpdate(ControllableContainer * cc, Controllable * c) override;
	
	void run() override;
};

