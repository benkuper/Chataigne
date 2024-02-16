/*
  ==============================================================================

	MIDIDevice.cpp
	Created: 20 Dec 2016 1:17:56pm
	Author:  Ben

  ==============================================================================
*/

MIDIDevice::MIDIDevice(const MidiDeviceInfo& info, Type t) :
	id(info.identifier),
	name(info.name),
	type(t)
{}



MIDIInputDevice::MIDIInputDevice(const MidiDeviceInfo & info) :
	MIDIDevice(info, MIDI_IN)
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
		//int deviceIndex = MidiInput::getDevices().indexOf(name);
		device.reset();
		device = MidiInput::openDevice(id, this);

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
  else if (message.isProgramChange()) inputListeners.call(&MIDIInputListener::programChangeReceived, message.getChannel(), message.getProgramChangeNumber());
	else if (message.isFullFrame()) inputListeners.call(&MIDIInputListener::fullFrameTimecodeReceived, message);
	else if (message.isQuarterFrame()) inputListeners.call(&MIDIInputListener::quarterFrameTimecodeReceived, message);
	else if (message.isPitchWheel()) inputListeners.call(&MIDIInputListener::pitchWheelReceived, message.getChannel(), message.getPitchWheelValue());
	else if (message.isChannelPressure()) inputListeners.call(&MIDIInputListener::channelPressureReceived, message.getChannel(), message.getChannelPressureValue());
	else if (message.isAftertouch()) inputListeners.call(&MIDIInputListener::afterTouchReceived, message.getChannel(), message.getNoteNumber(), message.getAfterTouchValue());
	else if (message.isMidiClock()) inputListeners.call(&MIDIInputListener::midiClockReceived);
	else if (message.isMidiStart()) inputListeners.call(&MIDIInputListener::midiStartReceived);
	else if (message.isMidiStop()) inputListeners.call(&MIDIInputListener::midiStopReceived);
	else if (message.isMidiContinue()) inputListeners.call(&MIDIInputListener::midiContinueReceived);
	else if (message.isMidiMachineControlMessage()) inputListeners.call(&MIDIInputListener::midiMachineControlCommandReceived, message.getMidiMachineControlCommand());
	else if (message.isSysEx()) inputListeners.call(&MIDIInputListener::sysExReceived, message);
	else
	{
		int hours, minutes, seconds, frames;
		if (message.isMidiMachineControlGoto(hours, minutes, seconds, frames)) inputListeners.call(&MIDIInputListener::midiMachineControlGotoReceived, hours, minutes, seconds, frames);
		else
		{
			DBG("Not handled : " << message.getDescription());
		}
	}
}






//*****************   MIDI OUTPUT


MIDIOutputDevice::MIDIOutputDevice(const MidiDeviceInfo & info) :
	MIDIDevice(info, MIDI_OUT),
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
		//int deviceIndex = MidiOutput::getAvailableDevices().indexOf(name);
		device.reset();
		device = MidiOutput::openDevice(id);// deviceIndex);
		if (device != nullptr)
		{
			LOG("MIDI Out " << device->getName() << " opened");
		} else
		{
			LOGERROR("MIDI Out " << name << " open error");
		}
	}
}

void MIDIOutputDevice::close()
{
	usageCount--;
	if (usageCount == 0)
	{
		device = nullptr;
		LOG("MIDI Out " << name << " closed");
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

void MIDIOutputDevice::sendMidiMachineControlGoto(int hours, int minutes, int seconds, int frames)
{
	if (device == nullptr) return;
	device->sendMessageNow(MidiMessage::midiMachineControlGoto(hours, minutes, seconds, frames));
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
