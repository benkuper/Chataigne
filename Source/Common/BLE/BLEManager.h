/*
  ==============================================================================

	BLEManager.h
	Created: 1 Feb 2023 1:22:50pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

#ifndef BLE_SUPPORT
	#define BLE_SUPPORT 0
#endif

#if BLE_SUPPORT

class BLEManager :
	public Thread
{
public:
	juce_DeclareSingleton(BLEManager, true);
	BLEManager();
	~BLEManager();


	OwnedArray<BLEDevice> devices;


	std::optional<Adapter> adapter;
	
	void setup();
	void updateDeviceList();

	BLEDevice* getDevice(Peripheral& p);
	BLEDevice* getDevice(String deviceID);

	void addDevice(Peripheral& p);
	void removeDevice(BLEDevice* p);

	void run() override;



	class BLEManagerListener
	{
	public:
		virtual ~BLEManagerListener() {}
		virtual void bleDeviceAdded(BLEDevice*) = 0;
		virtual void bleDeviceRemoved(BLEDevice*) = 0;
	};

	ListenerList<BLEManagerListener> listeners;
	void addBLEManagerListener(BLEManagerListener* newListener) { listeners.add(newListener); }
	void removeBLEManagerListener(BLEManagerListener* listener) { listeners.remove(listener); }

};

#endif