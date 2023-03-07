/*
  ==============================================================================

	BLEDevice.cpp
	Created: 1 Feb 2023 1:22:59pm
	Author:  bkupe

  ==============================================================================
*/

#include "Common/CommonIncludes.h"

#if BLE_SUPPORT

BLEDevice::BLEDevice(Peripheral& p, DeviceMode _mode) :
	peripheral(p),
	mode(_mode),
	thread("BLE " + p.identifier(), this)
{
	name = p.identifier();
	id = p.address();
	description = name + " (" + id + ")";
}

BLEDevice::~BLEDevice()
{
	//cleanup
	close();
	listeners.call(&BLEDeviceListener::deviceRemoved, this);
}

void BLEDevice::setMode(DeviceMode _mode)
{
	if (mode == _mode) return; //do nothing if the same

	if (_mode == LINES) //must restart to make sure thread is not hanging in readLine
	{
		if (thread.isThreadRunning())
		{
			thread.stopThread(1000);
			thread.startThread();
		}
	}

	mode = _mode;
}

void BLEDevice::setPeripheral(Peripheral& p)
{
	GenericScopedLock lock(peripheralLock);
	peripheral = p;
}

void BLEDevice::open()
{
	if (thread.isThreadRunning()) return;

	thread.startThread();
}

void BLEDevice::close()
{
	thread.removeBLEListener(this);
	thread.stopThread(1000);

	try
	{
		peripheral.disconnect();

	}
	catch (std::exception e)
	{
		NLOGWARNING("BLE", "Error closing device : " << description);
	}

	listeners.call(&BLEDeviceListener::deviceClosed, this);
}

bool BLEDevice::isOpen() {
	GenericScopedLock lock(peripheralLock);
	return peripheral.is_connected();
}

int BLEDevice::writeString(String message, String serviceUUID, String writeUUID)
{
	if (!isOpen()) return 0;

	try
	{
		GenericScopedLock lock(peripheralLock);
		peripheral.write_command(serviceUUID.toStdString(), writeUUID.toStdString(), message.toRawUTF8());
		return 1;
	}
	catch (std::exception e)
	{
		LOGWARNING("Error writing to ble : " << e.what());
	}

	return 0;
}


int BLEDevice::writeBytes(Array<uint8_t> data, String serviceUUID, String writeUUID)
{
	try
	{
		GenericScopedLock lock(peripheralLock);
		peripheral.write_command(serviceUUID.toStdString(), writeUUID.toStdString(), (char*)data.getRawDataPointer());
		return 1;
	}
	catch (std::exception e)
	{
		NLOGERROR("BLE", "Error writing to device : " << e.what());
	}

	return 0;
}

void BLEDevice::dataReceived(const var& data) {
	listeners.call(&BLEDeviceListener::bleDataReceived, data);
}

void BLEDevice::addBLEDeviceListener(BLEDeviceListener* newListener) { listeners.add(newListener); }

void BLEDevice::removeBLEDeviceListener(BLEDeviceListener* listener) { listeners.remove(listener); }

BLEReadThread::BLEReadThread(String name, BLEDevice* _device) :
	Thread(name + "_thread"),
	device(_device)
{
}

BLEReadThread::~BLEReadThread()
{
	stopThread(1000);
}

void BLEReadThread::run()
{
	LOG("Connecting to BLE device " << device->description << "...");

	device->peripheral.connect();

	if (!device->isOpen())
	{
		NLOGERROR("BLE", "Error opening device " << device->description);
		return;
	}

	NLOG("BLE", "Device opened : " << device->description);
	device->listeners.call(&BLEDevice::BLEDeviceListener::deviceOpened, device);

	std::vector<uint8_t> byteBuffer; //for cobs and data255

	while (!threadShouldExit())
	{
		sleep(2); //500fps

		if (device == nullptr) return;
		if (!device->isOpen()) return;

		try
		{

			//device->peripheral.read()
			//size_t numBytes = (int)device->peripheral.available();
			//if (numBytes == 0) continue;

			//switch (device->mode)
			//{

			//case BLEDevice::DeviceMode::LINES:
			//{
			//	while (device->device->available() && device->mode == BLEDevice::DeviceMode::LINES)
			//	{
			//		std::string line = device->device->readline();
			//		if (line.size() > 0) bleThreadListeners.call(&BLEThreadListener::dataReceived, var(line));
			//	}
			//}
			//break;

			//case BLEDevice::DeviceMode::DIRECT:
			//{
			//	std::vector<uint8_t> data;
			//	device->device->read(data, numBytes);
			//	String s(std::string(data.begin(), data.end()));
			//	bleThreadListeners.call(&BLEThreadListener::dataReceived, var(s));
			//}
			//break;

			//case BLEDevice::DeviceMode::RAW:
			//{
			//	std::vector<uint8_t> data;
			//	device->device->read(data, numBytes);
			//	//for (int i = 0; i < data.size(); ++i) DBG("Data " << data[i]);
			//	bleThreadListeners.call(&BLEThreadListener::dataReceived, var(data.data(), numBytes));
			//}
			//break;

			//case BLEDevice::DeviceMode::DATA255:
			//{
			//	while (device->device->available() && device->mode == BLEDevice::DeviceMode::DATA255)
			//	{
			//		uint8_t b = device->device->read(1)[0];
			//		if (b == 255)
			//		{
			//			bleThreadListeners.call(&BLEThreadListener::dataReceived, var(byteBuffer.data(), byteBuffer.size()));
			//			byteBuffer.clear();
			//		}
			//		else
			//		{
			//			byteBuffer.push_back(b);
			//		}
			//	}
			//}
			//break;


			//case BLEDevice::DeviceMode::JSON:
			//{
			//	std::vector<uint8_t> data;
			//	device->device->read(data, numBytes);
			//	byteBuffer.insert(byteBuffer.end(), data.begin(), data.end());

			//	String s(std::string(byteBuffer.begin(), byteBuffer.end()));
			//	var o = JSON::parse(s);
			//	if (o.isObject())
			//	{
			//		bleThreadListeners.call(&BLEThreadListener::dataReceived, var(s));
			//		byteBuffer.clear();
			//	}
			//}
			//break;

			//case BLEDevice::DeviceMode::COBS:
			//{
			//	while (device->available() && device->mode == BLEDevice::DeviceMode::COBS)
			//	{
			//		uint8_t b = device->device->read(1)[0];
			//		byteBuffer.push_back(b);
			//		if (b == 0)
			//		{
			//			uint8_t decodedData[255];
			//			size_t numDecoded = cobs_decode(byteBuffer.data(), byteBuffer.size(), decodedData);
			//			bleThreadListeners.call(&BLEThreadListener::dataReceived, var(decodedData, numDecoded - 1));
			//			byteBuffer.clear();
			//		}
			//	}
			//}
			//break;
			//}
		}
		catch (...)
		{
			DBG("### BLE Problem ");
		}
	}

	device->peripheral.disconnect();
	device->listeners.call(&BLEDevice::BLEDeviceListener::deviceClosed, device);

	DBG("END BLE THREAD");
}

#endif