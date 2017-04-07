/*
  ==============================================================================

    SerialModule.cpp
    Created: 15 Mar 2017 10:17:22am
    Author:  Ben-Portable

  ==============================================================================
*/

#include "SerialModule.h"
#include "ScriptManager.h"

SerialModule::SerialModule(const String &name) :
	Module(name),
	port(nullptr)
{
	portParam = new SerialDeviceParameter("Port", "Serial Port to connect",true);
	addParameter(portParam);
	
	modeParam = addEnumParameter("Mode", "Protocol for treating the incoming data");
	modeParam->addOption("Lines", SerialDevice::LINES);
	modeParam->addOption("Raw", SerialDevice::RAW);
	modeParam->addOption("Data255", SerialDevice::DATA255);
	modeParam->addOption("COBS", SerialDevice::COBS);

	scriptObject.setMethod(sendId, SerialModule::sendStringFromScript);
	scriptObject.setMethod(sendLineId, SerialModule::sendStringWithNewLineFromScript);
	scriptObject.setMethod(writeId, SerialModule::sendBytesFromScript);

	SerialManager::getInstance()->addSerialManagerListener(this);
}

SerialModule::~SerialModule()
{
	if (SerialManager::getInstanceWithoutCreating() != nullptr)
	{
		SerialManager::getInstance()->removeSerialManagerListener(this);
	}

	setCurrentPort(nullptr);
}

void SerialModule::setCurrentPort(SerialDevice * _port)
{
	if (port == _port) return;

	if (port != nullptr)
	{
		port->removeSerialDeviceListener(this);
	}

	port = _port;

	if (port != nullptr)
	{
		port->addSerialDeviceListener(this);
		port->open();
		if (!port->isOpen())
		{
			NLOG(niceName, "Could not open port : " << port->info->port);
		}
		else
		{
			port->mode = (SerialDevice::PortMode)(int)modeParam->getValueData();
		}
		lastOpenedPortID = port->info->port;
	}

	serialModuleListeners.call(&SerialModuleListener::currentPortChanged);
}


void SerialModule::onContainerParameterChangedInternal(Parameter * p) {
	Module::onContainerParameterChangedInternal(p);
	
	if (p == portParam)
	{
		 setCurrentPort(portParam->getDevice());
	}if (p == modeParam)
	{
		if (port != nullptr) port->mode = (SerialDevice::PortMode)(int)modeParam->getValueData();
	}
};


void SerialModule::processDataLine(const String & message)
{
	if (logIncomingData->boolValue()) NLOG(niceName, "Message received :\n" + message);
	scriptManager->callFunctionOnAllItems(serialEventId,message);
}

void SerialModule::processDataBytes(Array<uint8_t> data)
{
	if (logIncomingData->boolValue())
	{
		String msg = "Message received :";
		for (auto &d : data) msg += "\n" + String(d);
		NLOG(niceName, msg);
	}

	if (scriptManager->items.size() > 0)
	{
		var args;
		for (auto &d : data) args.append(d);
		scriptManager->callFunctionOnAllItems(serialEventId, args);
	}
	
}


void SerialModule::portOpened(SerialDevice *)
{
	serialModuleListeners.call(&SerialModuleListener::portOpened);

}

void SerialModule::portClosed(SerialDevice *)
{
	serialModuleListeners.call(&SerialModuleListener::portClosed);
}

void SerialModule::portRemoved(SerialDevice *)
{
	setCurrentPort(nullptr);
}

void SerialModule::serialDataReceived(const var & data)
{
	inActivityTrigger->trigger();
	
	switch (port->mode)
	{

	case SerialDevice::LINES:
		processDataLine(data.toString());
		break;

	case SerialDevice::DATA255:
	case SerialDevice::RAW:
	case SerialDevice::COBS:
		Array<uint8> bytes((const uint8_t *)data.getBinaryData()->getData(), data.getBinaryData()->getSize());
		processDataBytes(bytes);
	break;

	}
	
}

var SerialModule::sendStringFromScript(const var::NativeFunctionArgs & a)
{
	SerialModule * m = getObjectFromJS<SerialModule>(a);
	if (!m->enabled->boolValue()) return var();
	if (m->port == nullptr) return var();

	m->port->writeString(a.arguments[0].toString(),false);
	return var();
}

var SerialModule::sendStringWithNewLineFromScript(const var::NativeFunctionArgs & a)
{
	SerialModule * m = getObjectFromJS<SerialModule>(a);
	if (!m->enabled->boolValue()) return var();
	if (m->port == nullptr) return var();

	m->port->writeString(a.arguments[0].toString(),true);
	return var();
}


var SerialModule::sendBytesFromScript(const var::NativeFunctionArgs & a)
{
	SerialModule * m = getObjectFromJS<SerialModule>(a);
	if (!m->enabled->boolValue()) return var();
	if (m->port == nullptr) return var();
	
	Array<uint8> data;
	for (int i = 0; i < a.numArguments; i++)
	{
		if (a.arguments[i].isArray())
		{
			Array<var> * aa = a.arguments[i].getArray();
			for (auto &vaa : *aa) data.add((uint8)(int)vaa);
		}
		else if(a.arguments[i].isInt())
		{
			data.add((uint8)(int)a.arguments[i]);
		}
	}
	m->port->writeBytes(data);
	return var();
}


void SerialModule::portAdded(SerialDeviceInfo * info)
{
	//DBG("SerialModule, portAdded >" << info->hardwareID << "< > " << lastOpenedPortID);
	if (port == nullptr && lastOpenedPortID == info->port)
	{
		setCurrentPort(SerialManager::getInstance()->getPort(info));
	}
}

void SerialModule::portRemoved(SerialDeviceInfo *)
{
}