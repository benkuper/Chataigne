/*
  ==============================================================================

	SerialDevice.cpp
	Created: 15 Mar 2017 10:15:01am
	Author:  Ben-Portable

  ==============================================================================
*/

#include "SerialDevice.h"
#include "SerialManager.h"

#if SERIALSUPPORT
SerialDevice::SerialDevice(Serial * _port, SerialDeviceInfo  * _info, PortMode _mode) :
thread(_info->port, this),
port(_port),
info(_info),
mode(_mode)
{
	open();

	
}
#else
SerialDevice::SerialDevice(SerialDeviceInfo  * _info, PortMode _mode) :
	info(_info),
	mode(_mode),
	thread(_info->port, this)
{
	open();
}
#endif

SerialDevice::~SerialDevice()
{
	//cleanup
	close();
	listeners.call(&SerialDeviceListener::portRemoved, this);
}

void SerialDevice::open()
{
#if SERIALSUPPORT
	if (!port->isOpen())
	{
		port->open();
	}

	port->setDTR();
	port->setRTS();

	if (!thread.isThreadRunning())
	{
		thread.startThread();
#ifdef SYNCHRONOUS_SERIAL_LISTENERS
		thread.addSerialListener(this);
#else
		thread.addAsyncSerialListener(this);
#endif
		listeners.call(&SerialDeviceListener::portOpened, this);
	}
#endif
}

void SerialDevice::close()
{
#if SERIALSUPPORT
	if (port->isOpen())
	{
#ifdef SYNCHRONOUS_SERIAL_LISTENERS
		thread.removeSerialListener(this);
#else
		thread.removeAsyncSerialListener(this);
#endif

		thread.signalThreadShouldExit();
		while (thread.isThreadRunning());

		port->close();
		listeners.call(&SerialDeviceListener::portClosed, this);

	}
#endif
}

bool SerialDevice::isOpen() {
#if SERIALSUPPORT
	if (port == nullptr) return false;
	return port->isOpen();
#else
	return false;
#endif
}

int SerialDevice::writeString(String message, bool endLine)
{
#if SERIALSUPPORT
	if (!port->isOpen()) return 0;

	//DBG("Write string : " << message << " -- endline ? " << String(endLine));
	String m = message;
	if (endLine) m += "\n";
	return (int)port->write(m.toStdString());
#else
	return 0;
#endif
}

int SerialDevice::writeBytes(Array<uint8_t> data)
{
#if SERIALSUPPORT
	return (int)port->write(data.getRawDataPointer(), data.size());
#else
	return 0;
#endif
}

void SerialDevice::newMessage(const var & data) {
	listeners.call(&SerialDeviceListener::serialDataReceived, data);
}

void SerialDevice::addSerialDeviceListener(SerialDeviceListener * newListener) { listeners.add(newListener); }

void SerialDevice::removeSerialDeviceListener(SerialDeviceListener * listener) {
	listeners.remove(listener);
	if (listeners.size() == 0) SerialManager::getInstance()->removePort(this);
}

SerialReadThread::SerialReadThread(String name, SerialDevice * _port) :
	Thread(name + "_thread"),
	port(_port)
{
}

SerialReadThread::~SerialReadThread()
{
	signalThreadShouldExit();
	while (isThreadRunning());
}

void SerialReadThread::run()
{
#if SERIALSUPPORT
	
	std::vector<uint8_t> byteBuffer; //for cobs and data255

	while (!threadShouldExit())
	{
		sleep(10); //100fps

		if (port == nullptr) return;
		if (!port->isOpen()) return;

		try
		{

			size_t numBytes = (int)port->port->available();
			if (numBytes == 0) continue;

			switch (port->mode)
			{

			case SerialDevice::PortMode::LINES:
			{

				std::string line = port->port->readline();
				if (line.size() > 0)
				{
					serialThreadListeners.call(&SerialThreadListener::newMessage, var(line));
				}

			}
			break;

			case SerialDevice::PortMode::RAW:
			{
				std::vector<uint8_t> data;
				port->port->read(data,numBytes);
				serialThreadListeners.call(&SerialThreadListener::newMessage, var(data.data(),numBytes));
			}
			break;

			case SerialDevice::PortMode::DATA255:
			{
				while (port->port->available())
				{
					uint8_t b = port->port->read(1)[0];
					if (b == 255)
					{
						serialThreadListeners.call(&SerialThreadListener::newMessage, var(byteBuffer.data(),byteBuffer.size()));
						byteBuffer.clear();
					}
					else
					{
						byteBuffer.push_back(b);
					}
				}
			}
			break;

			case SerialDevice::PortMode::COBS:
			{
				while (port->port->available())
				{
					uint8_t b = port->port->read(1)[0];
					byteBuffer.push_back(b);
					if (b == 0)
					{
						uint8_t decodedData[255];
						size_t numDecoded = cobs_decode(byteBuffer.data(), byteBuffer.size(), decodedData); 
						serialThreadListeners.call(&SerialThreadListener::newMessage, var(decodedData, numDecoded));
						byteBuffer.clear();
					}
				}				
			}
			break;
			}
		}
		catch (...)
		{
			DBG("### Serial Problem ");
		}


	}
#endif

}
