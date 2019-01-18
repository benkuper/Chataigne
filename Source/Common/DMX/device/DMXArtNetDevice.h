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

#define OP_CODE 0x5000
#define PROTOCOL_VERSION 14
#define NUM_CHANNELS 512

class DMXArtNetDevice :
	public DMXDevice
{
public:
	DMXArtNetDevice();
	~DMXArtNetDevice();

	StringParameter * nodeIP;
	IntParameter * nodePort;

	DatagramSocket artNetOut;

	int sequence;
	
	const uint8 header[18]{ 'A', 'r', 't', '-', 'N', 'e', 't', 0,
					  OP_CODE & 255,(OP_CODE >> 8) & 255, (PROTOCOL_VERSION >> 8) & 255,PROTOCOL_VERSION & 255,
					  0,0,0,0,
					  (NUM_CHANNELS >> 8) & 255,NUM_CHANNELS & 255};

	uint8 fullMessage[NUM_CHANNELS + 18];

	void sendDMXValue(int channel, int value) override;
	void sendDMXRange(int startChannel, Array<int> values) override;

	void sendDMXValues() override;
};



#endif  // DMXARTNETDEVICE_H_INCLUDED
