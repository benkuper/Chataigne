/*
  ==============================================================================

    BLEModule.h
    Created: 1 Feb 2023 4:50:15pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once
#if BLE_SUPPORT

class BLEModule :
	public StreamingModule,
	public BLEDevice::BLEDeviceListener,
	public BLEManager::BLEManagerListener
{
public:
	BLEModule(const String& name = "Bluetooth LE");
	virtual ~BLEModule();


	//Device info

	StringParameter* nameFilter;
	StringParameter* serviceUUID;
	StringParameter* readUUID;
	StringParameter* writeUUID;

	String deviceID;
	String lastOpenedDeviceID; //for ghosting

	BLEDeviceParameter* deviceParam;
	BLEDevice* device;
	BoolParameter* isConnected;

	virtual void setCurrentDevice(BLEDevice* device, bool force = false);
	virtual void setupDeviceInternal() {}

	virtual void deviceOpenedInternal() {}
	virtual void deviceClosedInternal() {}

	virtual void onContainerParameterChangedInternal(Parameter* p) override;
	virtual void onControllableFeedbackUpdateInternal(ControllableContainer*, Controllable* c) override;

	virtual bool isReadyToSend() override;
	virtual void sendMessageInternal(const String& message, var) override;
	virtual void sendBytesInternal(Array<uint8> data, var) override;


	// Inherited via BLEDeviceListener
	virtual void deviceOpened(BLEDevice*) override;
	virtual void deviceClosed(BLEDevice*) override;
	virtual void deviceRemoved(BLEDevice*) override;
	virtual void bleDataReceived(const var& data) override;

	// Inherited via BLEManagerListener
	virtual void bleDeviceAdded(BLEDevice* device) override;
	virtual void bleDeviceRemoved(BLEDevice* device) override;

	virtual var getJSONData() override;
	virtual void loadJSONDataInternal(var data) override;

	virtual void setupModuleFromJSONData(var data) override;

	class BLEModuleListener
	{
	public:
		virtual ~BLEModuleListener() {}
		virtual void deviceOpened() {}
		virtual void deviceClosed() {}
		virtual void currentDeviceChanged() {}
	};

	ListenerList<BLEModuleListener> bleModuleListeners;
	void addBLEModuleListener(BLEModuleListener* newListener) { bleModuleListeners.add(newListener); }
	void removeBLEModuleListener(BLEModuleListener* listener) { bleModuleListeners.remove(listener); }

	static BLEModule* create() { return new BLEModule(); }
	virtual String getDefaultTypeString() const override { return "Bluetooth LE"; }

};

#endif