/*
  ==============================================================================

    DMXArtNetDevice.h
    Created: 10 Apr 2017 12:44:42pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#define DMX_OPCODE 0x5000
#define DMX_SYNC_OPCODE 0x5200
#define PROTOCOL_VERSION 14
#define NUM_CHANNELS 512
#define DMX_HEADER_LENGTH 18
#define MAX_PACKET_LENGTH DMX_HEADER_LENGTH+NUM_CHANNELS

class DMXArtNetDevice :
	public DMXDevice,
	//public EngineListener,
	public Thread //receiving
{
public:
	DMXArtNetDevice();
	~DMXArtNetDevice();

	//EnumParameter * networkInterface;
	IntParameter* localPort;
	IntParameter* inputNet;
	IntParameter* inputSubnet;
	IntParameter* inputUniverse;
	//StringParameter * nodeName;

	StringParameter* remoteHost;
	IntParameter * remotePort;
	IntParameter* outputNet;
	IntParameter* outputSubnet;
	IntParameter* outputUniverse;

	std::unique_ptr<DatagramSocket> receiver;
	DatagramSocket sender;

	uint8 sequenceNumber;
	uint8 artnetPacket[MAX_PACKET_LENGTH]{ 'A','r','t','-','N','e','t',0, 0x00 , 0x50,  0, PROTOCOL_VERSION };
	uint8 artextPacket[MAX_PACKET_LENGTH]{ 'A','r','t','-','E','x','t',0, 0x00 , 0x50,  0, PROTOCOL_VERSION };
	uint8 receiveBuffer[MAX_PACKET_LENGTH];


	void setupReceiver();

	void sendDMXValue(int channel, int value) override;
	void sendDMXRange(int startChannel, Array<int> values) override;

	void sendDMXValuesInternal() override;

//	void endLoadFile() override;

	void onControllableFeedbackUpdate(ControllableContainer * cc, Controllable * c) override;
	
	void run() override;
};

