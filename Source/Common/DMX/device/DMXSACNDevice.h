/*
  ==============================================================================

	DMXSACNDevice.h
	Created: 7 Apr 2021 7:49:19pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

#define NUM_CHANNELS 512

#pragma warning(push) 
#pragma warning(disable:4201) 
#include "sacn/e131.h"
#pragma warning(pop)

class DMXSACNDevice :
	public DMXDevice,
	//	public EngineListener,
	public Thread //receiving
{
public:
	DMXSACNDevice();
	~DMXSACNDevice();

	//EnumParameter * networkInterface;
	IntParameter* localPort;
	//BoolParameter* receiveMulticast;
	//IntParameter* inputUniverse;


	StringParameter* remoteHost;
	IntParameter* remotePort;
	StringParameter* nodeName;
	//BoolParameter* sendMulticast;
	//IntParameter* outputUniverse;
	IntParameter* priority;

	//Receiver
	std::unique_ptr<DatagramSocket> receiver;
	e131_packet_t receivedPacket;
	e131_error_t receivedError;
	uint8_t receivedSeq = 0x00;

	//Sender
	DatagramSocket sender;
	e131_packet_t senderPacket;
	e131_addr_t senderDest;

	void setupReceiver();
	void setupSender();

	//void sendDMXValue(int channel, int value) override;
	//void sendDMXRange(int startChannel, Array<int> values) override;

	void sendDMXValuesInternal(DMXUniverse* u) override;

	//	void endLoadFile() override;

	String getMulticastIPForUniverse(int universe) const;

	void onControllableFeedbackUpdate(ControllableContainer* cc, Controllable* c) override;

	void run() override;
};
