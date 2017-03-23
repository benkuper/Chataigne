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
	info(_info),
	port(_port),
	mode(_mode),
	thread(_info->port, this)
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

		thread.stopThread(10000);
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

	DBG("Write string : " << message << " -- endline ? " << String(endLine));
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
	port(_port),
	queuedNotifier(100)
{
}

SerialReadThread::~SerialReadThread()
{
	stopThread(100);
}

void SerialReadThread::run()
{
#if SERIALSUPPORT
	Array<int> byteBuffer;
	while (!threadShouldExit())
	{
		sleep(10); //100fps

		if (port == nullptr) return;
		if (!port->isOpen()) return;

		try
		{

			int numBytes = (int)port->port->available();
			if (numBytes == 0) continue;
			switch (port->mode)
			{

			case SerialDevice::PortMode::LINES:
			{

				std::string line = port->port->readline();
				if (line.size() > 0)
				{
					serialThreadListeners.call(&SerialThreadListener::newMessage, var(line));
					queuedNotifier.addMessage(new var(line));
				}

			}
			break;

			case SerialDevice::PortMode::RAW:
			{
				/*
				var * dataVar = new var();
				Array<int> bytes;
				while (port->port->available())
				{
				std::vector<uint8_t> data;
				port->port->read(data);
				bytes.addArray(data);
				for(auto &b:bytes) dataVar->append(b);
				}

				queuedNotifier.addMessage(dataVar);
				*/
			}
			break;

			case SerialDevice::PortMode::DATA255:
			{
				while (port->port->available())
				{
					uint8_t b = port->port->read(1)[0];
					if (b == 255)
					{
						var * dataVar = new var();
						for (auto &by : byteBuffer) dataVar->append(by);
						serialThreadListeners.call(&SerialThreadListener::newMessage, *dataVar);
						queuedNotifier.addMessage(dataVar);
						byteBuffer.clear();
					}
					else
					{
						byteBuffer.add(b);
					}
				}
			}
			break;

			case SerialDevice::PortMode::COBS:
			{
				//todo : handle cobs
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