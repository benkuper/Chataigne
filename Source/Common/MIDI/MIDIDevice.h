/*
  ==============================================================================

    MIDIDevice.h
    Created: 20 Dec 2016 1:17:56pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class MIDIDevice
{
public:
	enum Type { MIDI_IN, MIDI_OUT };
	MIDIDevice(const String &deviceName, Type t);
	virtual ~MIDIDevice() {}
	String name;
	Type type;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MIDIDevice)
};

class MIDIInputDevice :
	public MIDIDevice,
	public MidiInputCallback
{
public:
	MIDIInputDevice(const String &deviceName);
	~MIDIInputDevice();
	std::unique_ptr<MidiInput> device;

	// Inherited via MidiInputCallback
	virtual void handleIncomingMidiMessage(MidiInput * source, const MidiMessage & message) override;

	class  MIDIInputListener
	{
	public:
		/** Destructor. */
		virtual ~MIDIInputListener() {}
		virtual void noteOnReceived(const int &/*channel*/, const int &/*pitch*/, const int &/*velocity*/) {}
		virtual void noteOffReceived(const int &/*channel*/, const int &/*pitch*/, const int &/*velocity*/) {}
		virtual void controlChangeReceived(const int &/*channel*/, const int &/*number*/, const int &/*value*/) {}
		virtual void sysExReceived(const MidiMessage &/*msg*/) {}
		virtual void fullFrameTimecodeReceived(const MidiMessage&/*msg*/) {}
		virtual void quarterFrameTimecodeReceived(const MidiMessage&/*msg*/) {}
		virtual void pitchWheelReceived(const MidiMessage &/*msg*/, const int &/*channel*/, const int &/*value*/){}
	};

	ListenerList<MIDIInputListener> inputListeners;
	void addMIDIInputListener(MIDIInputListener* newListener);
	void removeMIDIInputListener(MIDIInputListener* listener);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MIDIInputDevice)

};


class MIDIOutputDevice :
	public MIDIDevice
{
public:
	MIDIOutputDevice(const String &deviceName);
	~MIDIOutputDevice();

	std::unique_ptr<MidiOutput> device;
	int usageCount;

	void open();

	void close();

	void sendNoteOn(int channel, int pitch, int velocity);
	void sendNoteOff(int channel, int pitch);
	void sendControlChange(int channel, int number, int value);
	void sendProgramChange(int channel, int number);
	void sendSysEx(Array<uint8> data);
	void sendFullframeTimecode(int hours, int minutes, int seconds, int frames, MidiMessage::SmpteTimecodeType timecodeType);
	void sendQuarterframe(int piece, int value);
	void sendMidiMachineControlCommand(MidiMessage::MidiMachineControlCommand command);
	void sendPitchWheel(int channel, int value);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MIDIOutputDevice)
};
