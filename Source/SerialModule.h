/*
  ==============================================================================

    SerialModule.h
    Created: 15 Mar 2017 10:17:22am
    Author:  Ben-Portable

  ==============================================================================
*/

#ifndef SERIALMODULE_H_INCLUDED
#define SERIALMODULE_H_INCLUDED

#include "StreamingModule.h"
#include "SerialManager.h"
#include "SerialDeviceParameter.h"

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
	EnumParameter * modeParam;
	SerialDevice * port; 

	virtual void setCurrentPort(SerialDevice *port);
	virtual void onContainerParameterChangedInternal(Parameter * p) override;

	// Inherited via SerialDeviceListener
	virtual void portOpened(SerialDevice *) override;
	virtual void portClosed(SerialDevice *) override;
	virtual void portRemoved(SerialDevice *) override;
	virtual void serialDataReceived(const var &data) override;

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
