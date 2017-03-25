/*
  ==============================================================================

    SerialModule.h
    Created: 15 Mar 2017 10:17:22am
    Author:  Ben-Portable

  ==============================================================================
*/

#ifndef SERIALMODULE_H_INCLUDED
#define SERIALMODULE_H_INCLUDED

#include "Module.h"
#include "SerialManager.h"
#include "SerialDeviceParameter.h"

class SerialModule : 
	public Module,
	public SerialDevice::SerialDeviceListener,
	public SerialManager::SerialManagerListener
{
public:
	SerialModule(const String &name = "Serial");
	virtual ~SerialModule();

	String lastOpenedPortID; //for ghosting

	SerialDeviceParameter * portParam;
	EnumParameter * modeParam;

	SerialDevice * port; 

	//Script Identifier
	const Identifier serialEventId = "serialEvent";
	const Identifier sendId = "send";
	const Identifier sendLineId = "sendLine";
	const Identifier writeId = "write";

	void setCurrentPort(SerialDevice *port);

	void onContainerParameterChangedInternal(Parameter * p) override;

	//Device info
	String deviceID;

	//Serial functions
	virtual void processDataLine(const String &message);
	virtual void processDataBytes(Array<uint8> data);

	// Inherited via SerialDeviceListener
	virtual void portOpened(SerialDevice *) override;
	virtual void portClosed(SerialDevice *) override;
	virtual void portRemoved(SerialDevice *) override;
	virtual void serialDataReceived(const var &data) override;

	//Script
	static var sendStringFromScript(const var::NativeFunctionArgs &a);
	static var sendStringWithNewLineFromScript(const var::NativeFunctionArgs &a);
	static var sendBytesFromScript(const var::NativeFunctionArgs &a);



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
	virtual String getTypeString() const override { return "Serial"; }

};



#endif  // SERIALMODULE_H_INCLUDED
