/*
  ==============================================================================

	DMXEnttecProDevice.h
	Created: 7 Apr 2017 11:23:08am
	Author:  Ben

  ==============================================================================
*/

#pragma once

#define DMXPRO_START_MESSAGE 0x7E
#define DMXPRO_END_MESSAGE 0xE7

#define DMXPRO_RECEIVE_LABEL 5
#define DMXPRO_SEND_LABEL 6
#define DMXPRO_RECEIVE_ON_CHANGE_LABEL 8
#define DMXPRO_SERIAL_NUMBER_LABEL 10

#define DMXPRO_START_CODE 0

#define DMXPRO_CHANGE_ALWAYS_CODE 0
#define DMXPRO_HEADER_LENGTH 4


class DMXEnttecProDevice :
	public DMXSerialDevice
{
public:
	DMXEnttecProDevice();
	~DMXEnttecProDevice();

	Array<uint8> serialBuffer;

	uint8 sendHeaderData[5]{ DMXPRO_START_MESSAGE,DMXPRO_SEND_LABEL,(DMX_NUM_CHANNELS + 1) & 255,((DMX_NUM_CHANNELS + 1) >> 8) & 255, DMXPRO_START_CODE };
	uint8 sendFooterData[1]{ DMXPRO_END_MESSAGE };

	uint8 changeAlwaysData[6]{ DMXPRO_START_MESSAGE,DMXPRO_RECEIVE_ON_CHANGE_LABEL, 1, 0, DMXPRO_CHANGE_ALWAYS_CODE, DMXPRO_END_MESSAGE };

	void setPortConfig() override;
	void sendDMXValuesSerialInternal(DMXUniverse* u) override;


	void serialDataReceived(const var& data) override;
	Array<uint8> getDMXPacket(Array<uint8> bytes, int& endIndex);
	void processDMXPacket(Array<uint8> bytes);
	void readDMXPacket(Array<uint8> bytes, int expectedDataLength);
};