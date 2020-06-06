/*
  ==============================================================================

    DMXArtNetDevice.h
    Created: 10 Apr 2017 12:44:42pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "DMXDevice.h"


#define DMX_OPCODE 0x5000
#define PROTOCOL_VERSION 14
#define NUM_CHANNELS 512
#define DMX_HEADER_LENGTH 18
#define MAX_PACKET_LENGTH DMX_HEADER_LENGTH+NUM_CHANNELS

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

	std::unique_ptr<DatagramSocket> receiver;
	DatagramSocket sender;

	uint8 sequenceNumber;
	uint8 artnetPacket[MAX_PACKET_LENGTH]{ 'A','r','t','-','N','e','t',0, 0x00 , 0x50,  0, PROTOCOL_VERSION };
	uint8 receiveBuffer[MAX_PACKET_LENGTH];


	void setupReceiver();

	void sendDMXValue(int channel, int value) override;
	void sendDMXRange(int startChannel, Array<int> values) override;

	void sendDMXValues() override;

	void endLoadFile() override;

	void onContainerParameterChanged(Parameter * p) override;
	void onControllableStateChanged(Controllable* c) override;
	
	void run() override;
};

