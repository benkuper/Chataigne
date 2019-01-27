/*
  ==============================================================================

    SerialModule.h
    Created: 15 Mar 2017 10:17:22am
    Author:  Ben-Portable

  ==============================================================================
*/

#ifndef SERIALMODULE_H_INCLUDED
#define SERIALMODULE_H_INCLUDED

#include "../common/streaming/StreamingModule.h"
#include "Common/Serial/SerialManager.h"
#include "Common/Serial/SerialDeviceParameter.h"

class SerialModule : 
	public StreamingModule,
	public SerialDevice::SerialDeviceListener,
	public SerialManager::SerialManagerListener
{
public:
	SerialModule(const String &name = "Serial");
	virtual ~SerialModule();

	//Device info
	String deviceID;
	String lastOpenedPortID; //for ghosting

	SerialDeviceParameter * portParam;
	IntParameter * baudRate;
	SerialDevice * port; 
	BoolParameter * isConnected;

	virtual void setCurrentPort(SerialDevice *port);
	virtual void onControllableFeedbackUpdateInternal(ControllableContainer *, Controllable * c) override;

	virtual bool isReadyToSend() override;
	virtual void sendMessageInternal(const String &message) override;
	virtual void sendBytesInternal(Array<uint8> data) override;


	// Inherited via SerialDeviceListener
	virtual void portOpened(SerialDevice *) override;
	virtual void portClosed(SerialDevice *) override;
	virtual void portRemoved(SerialDevice *) override;
	virtual void serialDataReceived(const var &data) override;

	virtual var getJSONData() override;
	virtual void loadJSONDataInternal(var data) override;

	virtual void setupModuleFromJSONData(var data) override;

	class SerialModuleListener
	{
	public:
		virtual ~SerialModuleListener() {}
		virtual void portOpened() {}
		virtual void portClosed() {}
		virtual void currentPortChanged() {}
	};

	ListenerList<SerialModuleListener> serialModuleListeners;
	void addSerialModuleListener(SerialModuleListener* newListener) { serialModuleListeners.add(newListener); }
	void removeSerialModuleListener(SerialModuleListener* listener) { serialModuleListeners.remove(listener); }


	// Inherited via SerialManagerListener
	virtual void portAdded(SerialDeviceInfo * info) override;
	virtual void portRemoved(SerialDeviceInfo * info) override;


	static SerialModule * create() { return new SerialModule(); }
	virtual String getDefaultTypeString() const override { return "Serial"; }

};



#endif  // SERIALMODULE_H_INCLUDED
