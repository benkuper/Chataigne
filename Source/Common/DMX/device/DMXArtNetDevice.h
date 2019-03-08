/*
  ==============================================================================

    DMXArtNetDevice.h
    Created: 10 Apr 2017 12:44:42pm
    Author:  Ben

  ==============================================================================
*/

#ifndef DMXARTNETDEVICE_H_INCLUDED
#define DMXARTNETDEVICE_H_INCLUDED

#include "DMXDevice.h"
#include "artnet.h"

#define OP_CODE 0x5000
#define PROTOCOL_VERSION 14
#define NUM_CHANNELS 512

class DMXArtNetDevice :
	public DMXDevice,
	public EngineListener
{
public:
	DMXArtNetDevice();
	~DMXArtNetDevice();

	EnumParameter * networkInterface; 

	StringParameter * nodeName;
	
	artnet_node ioNode;
	artnet_node discoverNode;
	int numFoundNodes;

	bool noServerCreation;

	void setupReceiver();
	void setupNode();
	void discoverNodes();

	void sendDMXValue(int channel, int value) override;
	void sendDMXRange(int startChannel, Array<int> values) override;

	void sendDMXValues() override;

	static int artNetReplyHandler(artnet_node node, void * pp, void * devicePtr);
	static int artNetDMXReceiveHandler(artnet_node node, int port, void * data);


	void endLoadFile() override;

	struct NetworkInterface
	{
		String interfaceName;
		String ipAddress;
	};

	Array<NetworkInterface> getAllInterfaces();

	void onContainerParameterChanged(Parameter * p) override;
	void onControllableFeedbackUpdate(ControllableContainer * cc, Controllable * c) override;
	
	void runInternal() override;
};



#endif  // DMXARTNETDEVICE_H_INCLUDED
