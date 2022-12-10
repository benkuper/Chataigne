/*
  ==============================================================================

    DMXSerialDevice.h
    Created: 10 Apr 2017 10:58:01am
    Author:  Ben

  ==============================================================================
*/

#pragma once

class DMXSerialDevice :
	public DMXDevice,
	public SerialManager::SerialManagerListener,
	public SerialDevice::SerialDeviceListener
{

public:
	DMXSerialDevice(const String &name, Type type, bool canReceive);
	virtual ~DMXSerialDevice();

	bool setPortStatus(bool status);

	SerialDeviceParameter * portParam;
	SerialDevice * dmxPort;
	IntParameter * channelsParam;

	//Device info
	String deviceID;
	String lastOpenedPortID; //for ghosting

	void setCurrentPort(SerialDevice * port);
	virtual void setPortConfig() {}

	void refreshEnabled() override;

	virtual void processIncomingData();

	virtual void initRunLoop() {}
	virtual void sendDMXValuesInternal(DMXUniverse* u) override;
	virtual void sendDMXValuesSerialInternal(DMXUniverse* u) = 0;

	virtual void onContainerParameterChanged(Parameter * p) override;

	void portAdded(SerialDeviceInfo * info) override;
	void portRemoved(SerialDeviceInfo * info) override;

	virtual void portOpened(SerialDevice *) override;
	virtual void portClosed(SerialDevice *) override;
	virtual void portRemoved(SerialDevice *) override;
	virtual void serialDataReceived(const var &data) override;
};