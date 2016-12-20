/*
  ==============================================================================

    MIDIDevice.cpp
    Created: 20 Dec 2016 1:17:56pm
    Author:  Ben

  ==============================================================================
*/

#include "MIDIDevice.h"
#include "DebugHelpers.h"

 MIDIDevice::MIDIDevice(const String & deviceName, Type t) :
	name(deviceName),
	  type(t)
{}



 MIDIInputDevice::MIDIInputDevice(const String & deviceName) :
	 MIDIDevice(deviceName, IN)
 {
 }

 MIDIInputDevice::~MIDIInputDevice()
{
}

void MIDIInputDevice::addMIDIInputListener(MIDIInputListener * newListener)
{
	inputListeners.add(newListener);
	if (inputListeners.size() == 1)
	{
		int deviceIndex = MidiInput::getDevices().indexOf(name);
		device = MidiInput::openDevice(deviceIndex, this);
		if (device != nullptr)
		{
			device->start();
			LOG("MIDI In " << device->getName() << " opened");
		}
		else
		{
			LOG("MIDI In " << name << " open error !");
		}
	}
}

void MIDIInputDevice::removeMIDIInputListener(MIDIInputListener * listener) {
	inputListeners.remove(listener);
	if (inputListeners.size() == 0)
	{
		device->stop();
		device = nullptr;
		LOG("MIDI In " << name << " closed");
	}
}

void MIDIInputDevice::handleIncomingMidiMessage(MidiInput * source, const MidiMessage & message)
{
	if (source != device)
	{
		DBG("different device");
		return;
	}

	if (message.isNoteOn()) inputListeners.call(&MIDIInputListener::noteOnReceived, message.getChannel(), message.getNoteNumber(), message.getVelocity());
	else if(message.isNoteOff()) inputListeners.call(&MIDIInputListener::noteOffReceived, message.getChannel(), message.getNoteNumber(), 0); //force note off to velocity 0
	else if(message.isController()) inputListeners.call(&MIDIInputListener::noteOnReceived, message.getChannel(), message.getControllerNumber(), message.getControllerValue());
	else if(message.isSysEx()) inputListeners.call(&MIDIInputListener::sysExReceived, message);
}






//*****************   MIDI OUTPUT


MIDIOutputDevice::MIDIOutputDevice(const String & deviceName) :
	MIDIDevice(deviceName, OUT),
	usageCount(0)
{}

MIDIOutputDevice::~MIDIOutputDevice()
{
}

void MIDIOutputDevice::open()
{
	usageCount++;
	if (usageCount == 1)
	{
		int deviceIndex = MidiOutput::getDevices().indexOf(name);
		device = MidiOutput::openDevice(deviceIndex);
		if (device != nullptr)
		{
			LOG("MIDI Out " << device->getName() << " opened");
		}
		else
		{
			LOG("MIDI Out " << device->getName() << " open error");
		}
	}
}

void MIDIOutputDevice::close()
{
	usageCount--;
	if (usageCount == 0)
	{
		device = nullptr;
		LOG("MIDI In " << name << " closed");
	}
}

void MIDIOutputDevice::sendNoteOn(int pitch, int velocity, int channel)
{
	if (device == nullptr) return;
	device->sendMessageNow(MidiMessage::noteOn(channel, pitch, (uint8)velocity));
}

void MIDIOutputDevice::sendNoteOff(int pitch, int channel)
{
	if (device == nullptr) return;
	device->sendMessageNow(MidiMessage::noteOff(channel, pitch));
}

void MIDIOutputDevice::sendControlChange(int number, int value, int channel)
{
	if (device == nullptr) return;
	device->sendMessageNow(MidiMessage::controllerEvent(channel,number,value));
}

void MIDIOutputDevice::sendSysEx(uint8 d1, uint8 d2, uint8 d3, uint8 d4)
{
	if (device == nullptr) return;
	uint8 d[4]{ d1,d2,d3,d4 };
	device->sendMessageNow(MidiMessage::createSysExMessage(d, 4));
}

