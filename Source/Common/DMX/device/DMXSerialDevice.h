/*
  ==============================================================================

    DMXSerialDevice.h
    Created: 10 Apr 2017 10:58:01am
    Author:  Ben

  ==============================================================================
*/

#ifndef DMXSERIALDEVICE_H_INCLUDED
#define DMXSERIALDEVICE_H_INCLUDED

#include "DMXDevice.h"
#include "Common/Serial/SerialManager.h"
#include "Common/Serial/SerialDeviceParameter.h"

class DMXSerialDevice :
	public DMXDevice,
	public Thread,
	public SerialManager::SerialManagerListener,
	public SerialDevice::SerialDeviceListener
{

public:
	DMXSerialDevice(const String &name, Type type, bool canReceive);
	virtual ~DMXSerialDevice();

	SerialDeviceParameter * portParam;
	SerialDevice * dmxPort;

	//Device info
	String deviceID;
	String lastOpenedPortID; //for ghosting

	bool shouldSendData;

	void setCurrentPort(SerialDevice * port);
	virtual void setPortConfig() {}

	virtual void processIncomingData();
	virtual void sendDMXValue(int channel, int value) override;

	virtual void run() override;
	virtual void initRunLoop() {}
	virtual void sendDMXData() {}

	virtual void onContainerParameterChanged(Parameter * p) override;

	void portAdded(SerialDeviceInfo * info) override;
	void portRemoved(SerialDeviceInfo * info) override;

	virtual void portOpened(SerialDevice *) override;
	virtual void portClosed(SerialDevice *) override;
	virtual void portRemoved(SerialDevice *) override;
	virtual void serialDataReceived(const var &data) override;
};



#endif  // DMXSERIALDEVICE_H_INCLUDED
