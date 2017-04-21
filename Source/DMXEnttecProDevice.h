/*
  ==============================================================================

    DMXEnttecProDevice.h
    Created: 7 Apr 2017 11:23:08am
    Author:  Ben

  ==============================================================================
*/

#ifndef DMXENTTECPRODEVICE_H_INCLUDED
#define DMXENTTECPRODEVICE_H_INCLUDED

#include "DMXSerialDevice.h"

#define DMXPRO_START_MESSAGE 0x7E
#define DMXPRO_SEND_LABEL 6
#define DMXPRO_CHANNEL_COUNT 512
#define DMXPRO_END_MESSAGE 0xE7
#define DMXPRO_START_CODE 0

class DMXEnttecProDevice :
	public DMXSerialDevice
{
public:
	DMXEnttecProDevice();
	~DMXEnttecProDevice();

	uint8 sendHeaderData[5]{ DMXPRO_START_MESSAGE,DMXPRO_SEND_LABEL,(DMXPRO_CHANNEL_COUNT+1) & 255,((DMXPRO_CHANNEL_COUNT+1) >> 8) & 255,DMXPRO_START_CODE };
	uint8 sendFooterData[1]{ DMXPRO_END_MESSAGE };
	void setPortConfig();
	void sendDMXData();
};



#endif  // DMXENTTECPRODEVICE_H_INCLUDED
