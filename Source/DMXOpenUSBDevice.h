/*
  ==============================================================================

    DMXOpenUSBDevice.h
    Created: 7 Apr 2017 11:22:57am
    Author:  Ben

  ==============================================================================
*/

#ifndef DMXOPENUSBDEVICE_H_INCLUDED
#define DMXOPENUSBDEVICE_H_INCLUDED

#include "DMXDevice.h"
#include "SerialDevice.h"
#include "SerialDeviceParameter.h"

class DMXOpenUSBDevice :
	public DMXDevice,
	public Thread,
	public SerialManager::SerialManagerListener,
	public SerialDevice::SerialDeviceListener
{
public:
	DMXOpenUSBDevice();
	~DMXOpenUSBDevice();

	SerialDeviceParameter * portParam;
	SerialDevice * dmxPort;

	//Device info
	String deviceID;
	String lastOpenedPortID; //for ghosting


	void setCurrentPort(SerialDevice * port);


	virtual void run() override;

	virtual void onContainerParameterChanged(Parameter * p) override;
	
	void portAdded(SerialDeviceInfo * info) override;
	void portRemoved(SerialDeviceInfo * info) override;

	virtual void portOpened(SerialDevice *) override;
	virtual void portClosed(SerialDevice *) override;
	virtual void portRemoved(SerialDevice *) override;
	virtual void serialDataReceived(const var &data) override;

};



#endif  // DMXOPENUSBDEVICE_H_INCLUDED
