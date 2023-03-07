/*
  ==============================================================================

	BLEDevice.h
	Created: 1 Feb 2023 1:22:59pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

#if BLE_SUPPORT

class BLEDevice;

class BLEReadThread :
	public Thread
{
public:

	BLEReadThread(String name, BLEDevice* _device);
	virtual ~BLEReadThread();

	BLEDevice* device;


	virtual void run() override;


	class BLEThreadListener {
	public:
		virtual ~BLEThreadListener() {};
		virtual void dataReceived(const var&) {};
	};

	ListenerList<BLEThreadListener> serialThreadListeners;

	void addBLEListener(BLEThreadListener* newListener) { serialThreadListeners.add(newListener); }
	void removeBLEListener(BLEThreadListener* listener) { serialThreadListeners.remove(listener); }
};


class BLEDevice :
	public BLEReadThread::BLEThreadListener
{
public:
	BLEReadThread thread;

	enum DeviceMode { LINES = 0, DIRECT = 1, DATA255 = 2, RAW = 3, JSON = 4, COBS = 5 };

	BLEDevice(Peripheral& peripheral, DeviceMode mode = LINES);

	virtual ~BLEDevice();

	Peripheral peripheral;

	String name;
	String id;
	String description;

	DeviceMode mode;

	SpinLock peripheralLock;

	void setMode(DeviceMode mode);

	void setPeripheral(Peripheral& p);

	void open();
	void close();

	bool isOpen();

	//write functions
	int writeString(String message, String serviceUUID, String characteristics);
	int writeBytes(Array<uint8_t> data, String serviceUUID, String characteristics);

	virtual void dataReceived(const var& data) override;


	class BLEDeviceListener
	{
	public:
		virtual ~BLEDeviceListener() {}

		//serial data here
		virtual void deviceOpened(BLEDevice*) {};
		virtual void deviceClosed(BLEDevice*) {};
		virtual void deviceRemoved(BLEDevice* p) {};
		virtual void bleDataReceived(const var&) {};
	};

	ListenerList<BLEDeviceListener> listeners;
	void addBLEDeviceListener(BLEDeviceListener* newListener);
	void removeBLEDeviceListener(BLEDeviceListener* listener);
};

#endif