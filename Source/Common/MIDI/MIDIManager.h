/*
  ==============================================================================

	MIDIManager.h
	Created: 20 Dec 2016 12:33:33pm
	Author:  Ben

  ==============================================================================
*/

#pragma once

class MIDIManager
{
public:
	juce_DeclareSingleton(MIDIManager, true)
		MIDIManager();
	~MIDIManager();

	OwnedArray<MIDIInputDevice> inputs;
	OwnedArray<MIDIOutputDevice> outputs;

	void checkDevices();
	void addInputDeviceIfNotThere(const MidiDeviceInfo& info);
	void addOutputDeviceIfNotThere(const MidiDeviceInfo& info);
	void removeInputDevice(MIDIInputDevice* d);
	void removeOutputDevice(MIDIOutputDevice* d);


	MIDIInputDevice* getInputDeviceWithID(const String& id);
	MIDIOutputDevice* getOutputDeviceWithID(const String& id);

	MIDIInputDevice* getInputDeviceWithName(const String& name);
	MIDIOutputDevice* getOutputDeviceWithName(const String& name);

	//Gloabal Settings
	enum MIDIEventType { NOTE_ON, NOTE_OFF, FULL_NOTE, CONTROL_CHANGE, PITCH_WHEEL, SYSEX };
	EnumParameter* midiRouterDefaultType;

	class  Listener
	{
	public:
		/** Destructor. */
		virtual ~Listener() {}
		virtual void midiDeviceInAdded(MIDIInputDevice* /*input*/) {}
		virtual void midiDeviceOutAdded(MIDIOutputDevice* /*output*/) {}

		virtual void midiDeviceInRemoved(MIDIInputDevice* /*input*/) {}
		virtual void midiDeviceOutRemoved(MIDIOutputDevice* /*output*/) {}
	};

	ListenerList<Listener> listeners;
	void addMIDIManagerListener(Listener* newListener) { listeners.add(newListener); }
	void removeMIDIManagerListener(Listener* listener) { listeners.remove(listener); }


	static String getNoteName(const int& pitch, bool includeOctave = true, int octaveShift = 0)
	{
		return MidiMessage::getMidiNoteName(pitch, true, includeOctave, 3 - octaveShift);
	}

	MidiDeviceListConnection connection = MidiDeviceListConnection::make ([this]
	{
        checkDevices();
	});

	JUCE_DECLARE_NON_COPYABLE(MIDIManager)
};
