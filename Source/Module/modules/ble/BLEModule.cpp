/*
  ==============================================================================

	BLEModule.cpp
	Created: 1 Feb 2023 4:50:15pm
	Author:  bkupe

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

#if BLE_SUPPORT

BLEModule::BLEModule(const String& name) :
	StreamingModule(name),
	device(nullptr)
{
	deviceParam = new BLEDeviceParameter("Device", "BLE Device to connect", true);
	moduleParams.addParameter(deviceParam);

	serviceUUID = moduleParams.addStringParameter("Service UUID", "UUID for service to use", "6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
	writeUUID = moduleParams.addStringParameter("Write UUID", "UUID for service to use", "6E400002-B5A3-F393-E0A9-E50E24DCCA9E");
	readUUID = moduleParams.addStringParameter("Read UUID", "UUID for service to use", "6E400003-B5A3-F393-E0A9-E50E24DCCA9E");

	isConnected = moduleParams.addBoolParameter("Is Connected", "This is checked if a ble device is connected.", false);
	isConnected->setControllableFeedbackOnly(true);
	isConnected->isSavable = false;
	connectionFeedbackRef = isConnected;


	BLEManager::getInstance()->addBLEManagerListener(this);

}

BLEModule::~BLEModule()
{
	if (BLEManager::getInstanceWithoutCreating() != nullptr)
	{
		BLEManager::getInstance()->removeBLEManagerListener(this);
	}

	setCurrentDevice(nullptr);
}

void BLEModule::setCurrentDevice(BLEDevice* _device, bool force)
{
	if (device == _device && !force) return;

	if (device != nullptr)
	{
		device->close();
		deviceClosedInternal();
		device->removeBLEDeviceListener(this);
	}

	device = _device;

	if (device != nullptr)
	{
		device->addBLEDeviceListener(this);

		if (device->isOpen()) device->close();

		device->setMode(streamingType->getValueDataAsEnum<BLEDevice::DeviceMode>()); //always set mode, device might be already open with default mode
		setupDeviceInternal();
		device->open();
	}

	bleModuleListeners.call(&BLEModuleListener::currentDeviceChanged);
}

void BLEModule::onContainerParameterChangedInternal(Parameter* p)
{
	StreamingModule::onContainerParameterChangedInternal(p);
	if (p == enabled)
	{
		if (device != nullptr)
		{
			NLOG(niceName, "Module is " << (enabled->boolValue() ? "enabled" : "disabled") << ", " << (enabled->boolValue() ? "opening" : "closing ble device"));
			setCurrentDevice(enabled->boolValue() ? device : nullptr, true);
		}
	}
}

void BLEModule::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	StreamingModule::onControllableFeedbackUpdateInternal(cc, c);

	if (c == deviceParam)
	{
		BLEDevice* newDevice = deviceParam->getDevice();
		BLEDevice* prevDevice = device;
		setCurrentDevice(newDevice);

		if (device == nullptr && prevDevice != nullptr)
		{
			DBG("Manually set no ghost device");
			lastOpenedDeviceID = ""; //forces no ghosting when user chose to manually disable device
		}
	}if (c == streamingType)
	{
		if (device != nullptr) device->setMode((BLEDevice::DeviceMode)(int)streamingType->getValueData());
	}
}
bool BLEModule::isReadyToSend()
{
	if (device == nullptr)
	{
		NLOGWARNING(niceName, "You must set a device before sending data.");
		return false;
	}

	return true;
}
void BLEModule::sendMessageInternal(const String& message, var)
{
	if (device == nullptr) return;
	device->writeString(message, serviceUUID->stringValue(), writeUUID->stringValue());
}

void BLEModule::sendBytesInternal(Array<uint8> data, var)
{
	if (device == nullptr) return;
	device->writeBytes(data, serviceUUID->stringValue(), writeUUID->stringValue());
}


void BLEModule::deviceOpened(BLEDevice*)
{
	lastOpenedDeviceID = device->id;
	deviceOpenedInternal();

	LOG("Device " << device->description << " connected.");

	bleModuleListeners.call(&BLEModuleListener::deviceOpened);
}

void BLEModule::deviceClosed(BLEDevice*)
{
	deviceClosedInternal();
	bleModuleListeners.call(&BLEModuleListener::deviceClosed);
}

void BLEModule::deviceRemoved(BLEDevice*)
{
	setCurrentDevice(nullptr);
}

void BLEModule::bleDeviceAdded(BLEDevice* d)
{
	if (device == nullptr && lastOpenedDeviceID == d->id)
	{
		setCurrentDevice(d);
	}
}

void BLEModule::bleDeviceRemoved(BLEDevice* d)
{
	if (device != nullptr && device == d) setCurrentDevice(nullptr);
}

void BLEModule::bleDataReceived(const var& data)
{
	switch (device->mode)
	{

	case BLEDevice::LINES:
		processDataLine(data.toString());
		break;

	case BLEDevice::DATA255:
	case BLEDevice::RAW:
	case BLEDevice::COBS:
		if (data.isBinaryData() && data.getBinaryData() != nullptr)
		{
			Array<uint8> bytes((const uint8_t*)data.getBinaryData()->getData(), (int)data.getBinaryData()->getSize());
			processDataBytes(bytes);
		}
		else
		{
			NLOGWARNING(niceName, "Wrong data type detected, skipping");
		}
		break;

	}
}

var BLEModule::getJSONData()
{
	var data = StreamingModule::getJSONData();
	data.getDynamicObject()->setProperty("deviceID", lastOpenedDeviceID);
	return data;
}

void BLEModule::loadJSONDataInternal(var data)
{
	StreamingModule::loadJSONDataInternal(data);
	lastOpenedDeviceID = data.getProperty("deviceID", "");
}

void BLEModule::setupModuleFromJSONData(var data)
{
	if (data.hasProperty("nameFilter"))
	{
		var nameFilterData = data.getProperty("vidFilter", "");
		if (nameFilterData.isArray())
		{
			for (int i = 0; i < nameFilterData.size(); i++) deviceParam->nameFilters.add(nameFilterData[i].toString());
		}
		else if (nameFilterData.toString().isNotEmpty())
		{
			deviceParam->nameFilters.add(nameFilterData.toString());
		}
	}

	StreamingModule::setupModuleFromJSONData(data);
}

#endif