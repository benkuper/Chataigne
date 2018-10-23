/*
  ==============================================================================

    DMXOpenUSBDevice.h
    Created: 7 Apr 2017 11:22:57am
    Author:  Ben

  ==============================================================================
*/

#ifndef DMXOPENUSBDEVICE_H_INCLUDED
#define DMXOPENUSBDEVICE_H_INCLUDED

#include "DMXSerialDevice.h"

class DMXOpenUSBDevice :
	public DMXSerialDevice
{
public:
	DMXOpenUSBDevice();
	~DMXOpenUSBDevice();

	const uint8 startCode[1]{ 0 };

	void setPortConfig() override;
	void sendDMXValuesSerialInternal() override;
	
};



#endif  // DMXOPENUSBDEVICE_H_INCLUDED
