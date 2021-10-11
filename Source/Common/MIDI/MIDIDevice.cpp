/*
  ==============================================================================

	MIDIDevice.cpp
	Created: 20 Dec 2016 1:17:56pm
	Author:  Ben

  ==============================================================================
*/

MIDIDevice::MIDIDevice(const String & deviceName, Type t) :
	name(deviceName),
	type(t)
{}



MIDIInputDevice::MIDIInputDevice(const String & deviceName) :
	MIDIDevice(deviceName, MIDI_IN)
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
		device.reset();
		device = MidiInput::openDevice(deviceIndex, this);

		if (device != nullptr)
		{
			device->start();
			LOG("MIDI In " << device->getName() << " opened");
		} else
		{
			LOG("MIDI In " << name << " open error !");
		}
	}
}

void MIDIInputDevice::removeMIDIInputListener(MIDIInputListener * listener) {
	inputListeners.remove(listener);
	if (inputListeners.size() == 0)
	{
		if (device != nullptr) device->stop();
		device = nullptr;
		LOG("MIDI In " << name << " closed");
	}
}

void MIDIInputDevice::handleIncomingMidiMessage(MidiInput * source, const MidiMessage & message)
{
	if (source != device.get())
	{
		DBG("different device");
		return;
	}

	inputListeners.call(&MIDIInputListener::midiMessageReceived, message);

	if (message.isNoteOn()) inputListeners.call(&MIDIInputListener::noteOnReceived, message.getChannel(), message.getNoteNumber(), message.getVelocity());
	else if (message.isNoteOff()) inputListeners.call(&MIDIInputListener::noteOffReceived, message.getChannel(), message.getNoteNumber(), 0); //force note off to velocity 0
	else if (message.isController()) inputListeners.call(&MIDIInputListener::controlChangeReceived, message.getChannel(), message.getControllerNumber(), message.getControllerValue());
	else if (message.isSysEx()) inputListeners.call(&MIDIInputListener::sysExReceived, message);
	else if (message.isFullFrame()) inputListeners.call(&MIDIInputListener::fullFrameTimecodeReceived, message);
	else if (message.isQuarterFrame()) inputListeners.call(&MIDIInputListener::quarterFrameTimecodeReceived, message);
	else if (message.isPitchWheel()) inputListeners.call(&MIDIInputListener::pitchWheelReceived, message.getChannel(), message.getPitchWheelValue());
	else if (message.isChannelPressure()) inputListeners.call(&MIDIInputListener::channelPressureReceived, message.getChannel(), message.getChannelPressureValue());
	else if (message.isAftertouch()) inputListeners.call(&MIDIInputListener::afterTouchReceived, message.getChannel(), message.getNoteNumber(), message.getAfterTouchValue());
	else if (message.isMidiClock()) inputListeners.call(&MIDIInputListener::midiClockReceived);
	else if (message.isMidiStart()) inputListeners.call(&MIDIInputListener::midiStartReceived);
	else if (message.isMidiStop()) inputListeners.call(&MIDIInputListener::midiStopReceived);
	else if (message.isMidiContinue()) inputListeners.call(&MIDIInputListener::midiContinueReceived);
	else
	{
		DBG("Not handled : " << message.getDescription());
	}
}






//*****************   MIDI OUTPUT


MIDIOutputDevice::MIDIOutputDevice(const String & deviceName) :
	MIDIDevice(deviceName, MIDI_OUT),
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
		device.reset();
		device = MidiOutput::openDevice(deviceIndex);
		if (device != nullptr)
		{
			LOG("MIDI Out " << device->getName() << " opened");
		} else
		{
			LOG("MIDI Out " << name << " open error");
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

void MIDIOutputDevice::sendNoteOn(int channel, int pitch, int velocity)
{
	if (device == nullptr) return;
	device->sendMessageNow(MidiMessage::noteOn(channel, pitch, (uint8)velocity));
}

void MIDIOutputDevice::sendNoteOff(int channel, int pitch)
{
	if (device == nullptr) return;
	device->sendMessageNow(MidiMessage::noteOff(channel, pitch));
}

void MIDIOutputDevice::sendControlChange(int channel, int number, int value)
{
	if (device == nullptr) return;
	device->sendMessageNow(MidiMessage::controllerEvent(channel, number, value));
}

void MIDIOutputDevice::sendProgramChange(int channel, int number)
{
	if (device == nullptr) return;
	device->sendMessageNow(MidiMessage::programChange(channel, number));
}

void MIDIOutputDevice::sendSysEx(Array<uint8> data)
{
	if (device == nullptr) return;
	device->sendMessageNow(MidiMessage::createSysExMessage(data.getRawDataPointer(), data.size()));
}

void MIDIOutputDevice::sendFullframeTimecode(int hours, int minutes, int seconds, int frames, MidiMessage::SmpteTimecodeType timecodeType)
{
	if (device == nullptr) return;
	device->sendMessageNow(MidiMessage::fullFrame(hours, minutes, seconds, frames, timecodeType));
}

void MIDIOutputDevice::sendQuarterframe(int piece, int value)
{
	if (device == nullptr) return; 
	device->sendMessageNow(MidiMessage::quarterFrame(piece, value));
}

void MIDIOutputDevice::sendMidiMachineControlCommand(MidiMessage::MidiMachineControlCommand command)
{
	if (device == nullptr) return;
	device->sendMessageNow(MidiMessage::midiMachineControlCommand(command));
}

void MIDIOutputDevice::sendPitchWheel(int channel, int value)
{
	if (device == nullptr) return;
	device->sendMessageNow(MidiMessage::pitchWheel(channel, value));
}

void MIDIOutputDevice::sendChannelPressure(int channel, int value)
{
	if (device == nullptr) return;
	device->sendMessageNow(MidiMessage::channelPressureChange(channel, value));
}

void MIDIOutputDevice::sendAfterTouch(int channel, int note, int value)
{
	if (device == nullptr) return;
	device->sendMessageNow(MidiMessage::aftertouchChange(channel, note, value));
}