/*
  ==============================================================================

    SerialDevice.h
    Created: 15 Mar 2017 10:15:01am
    Author:  Ben-Portable

  ==============================================================================
*/

#pragma once

#define SYNCHRONOUS_SERIAL_LISTENERS

#define SERIALSUPPORT 1

#if SERIALSUPPORT
#include "serial/serial.h"
using namespace serial;
#endif

#include "lib/cobs/cobs.h"

#include "JuceHeader.h"

class SerialDevice;

class SerialReadThread :
	public Thread
{
public:

	SerialReadThread(String name, SerialDevice * _port);
	virtual ~SerialReadThread();

	SerialDevice * port;

	virtual void run() override;


	class SerialThreadListener {
	public:
		virtual ~SerialThreadListener() {};
		virtual void dataReceived(const var &) {};
	};

	ListenerList<SerialThreadListener> serialThreadListeners;

	void addSerialListener(SerialThreadListener* newListener) { serialThreadListeners.add(newListener); }
	void removeSerialListener(SerialThreadListener* listener) { serialThreadListeners.remove(listener); }
};

class SerialDeviceInfo
{
public:
	SerialDeviceInfo(String _port, String _description, String _hardwareID);

	virtual ~SerialDeviceInfo() {}

	String deviceID; //This is what we will use to identify the device uniquely, the way it's set depends on the OS

	String port;
	String description;
	String uniqueDescription;
	String hardwareID;
	int vid;
	int pid;
};

class SerialDevice :
#ifdef SYNCHRONOUS_SERIAL_LISTENERS
	public SerialReadThread::SerialThreadListener
#else
	public SerialReadThread::AsyncListener
#endif
{
public:
	SerialReadThread thread;

	enum PortMode { LINES, DATA255, RAW, COBS };

#if SERIALSUPPORT
	SerialDevice(Serial *port, SerialDeviceInfo * info, PortMode mode = LINES);
	std::unique_ptr<Serial> port;
#else
	SerialDevice(SerialDeviceInfo * info, PortMode mode = LINES);
#endif

	virtual ~SerialDevice();

	SerialDeviceInfo * info;

	bool openedOk; //have to have that because when erroring on openPort, port stills says it's open

	PortMode mode;
	void setMode(PortMode mode);
	void setBaudRate(int baudRate);

	void open(int baud = 9600);
	void close();


	bool isOpen();

	//write functions
	int writeString(String message);
	int writeBytes(Array<uint8_t> data);

	virtual void dataReceived(const var &data) override;

	class SerialDeviceListener
	{
	public:
		virtual ~SerialDeviceListener() {}

		//serial data here
		virtual void portOpened(SerialDevice  *) {};
		virtual void portClosed(SerialDevice  *) {};
		virtual void portRemoved(SerialDevice *) {};
		virtual void serialDataReceived(const var &) {};
	};

	ListenerList<SerialDeviceListener> listeners;
	void addSerialDeviceListener(SerialDeviceListener* newListener);
	void removeSerialDeviceListener(SerialDeviceListener* listener);
};