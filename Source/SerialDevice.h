/*
  ==============================================================================

    SerialDevice.h
    Created: 15 Mar 2017 10:15:01am
    Author:  Ben-Portable

  ==============================================================================
*/

#ifndef SERIALDEVICE_H_INCLUDED
#define SERIALDEVICE_H_INCLUDED


#define SYNCHRONOUS_SERIAL_LISTENERS

#if !defined __arm__
#define SERIALSUPPORT 1
#else
#define SERIALSUPPORT 0
#endif

#if SERIALSUPPORT
#include "serial/serial.h"
using namespace serial;
#endif

#include "cobs/cobs.h"

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
		virtual void newMessage(const var &) {};
	};

	ListenerList<SerialThreadListener> serialThreadListeners;

	void addSerialListener(SerialThreadListener* newListener) { serialThreadListeners.add(newListener); }
	void removeSerialListener(SerialThreadListener* listener) { serialThreadListeners.remove(listener); }
};

class SerialDeviceInfo
{
public:
	SerialDeviceInfo(String _port, String _description, String _hardwareID) :
		port(_port), description(_description), hardwareID(_hardwareID)
	{}

	virtual ~SerialDeviceInfo() {}

	String port;
	String description;
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
	ScopedPointer<Serial> port;
#else
	SerialDevice(SerialDeviceInfo * info, PortMode mode = LINES);
#endif

	virtual ~SerialDevice();

	SerialDeviceInfo * info;

	PortMode mode;

	void open();
	void close();

	bool isOpen();

	//write functions
	int writeString(String message, bool endLine = true);
	int writeBytes(Array<uint8_t> data);

	virtual void newMessage(const var &data) override;

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





#endif  // SERIALDEVICE_H_INCLUDED
