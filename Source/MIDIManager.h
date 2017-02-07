/*
  ==============================================================================

    MIDIManager.h
    Created: 20 Dec 2016 12:33:33pm
    Author:  Ben

  ==============================================================================
*/

#ifndef MIDIMANAGER_H_INCLUDED
#define MIDIMANAGER_H_INCLUDED

#include "JuceHeader.h"
#include "MIDIDevice.h"

class MIDIManager :
	public Timer
{
public:
	juce_DeclareSingleton(MIDIManager,true)
	MIDIManager();
	~MIDIManager();

	OwnedArray<MIDIInputDevice> inputs;
	OwnedArray<MIDIOutputDevice> outputs;

	void checkDevices();
	void addInputDeviceIfNotThere(const String &name);
	void addOutputDeviceIfNotThere(const String &name);
	void removeInputDevice(MIDIInputDevice * d);
	void removeOutputDevice(MIDIOutputDevice * d);

	MIDIInputDevice * getInputDeviceWithName(const String &name);
	MIDIOutputDevice * getOutputDeviceWithName(const String &name);
	
	
	class  Listener
	{
	public:
		/** Destructor. */
		virtual ~Listener() {}
		virtual void midiDeviceInAdded(MIDIInputDevice * /*input*/) = 0;
		virtual void midiDeviceOutAdded(MIDIOutputDevice * /*output*/) = 0;

		virtual void midiDeviceInRemoved(MIDIInputDevice * /*input*/) = 0;
		virtual void midiDeviceOutRemoved(MIDIOutputDevice * /*output*/) = 0;
	};

	ListenerList<Listener> listeners;
	void addMIDIManagerListener(Listener* newListener) { listeners.add(newListener); }
	void removeMIDIManagerListener(Listener* listener) { listeners.remove(listener); }



	// Inherited via Timer
	virtual void timerCallback() override;
	
	static String getNoteName(const int &pitch, bool includeOctave = true)
	{
		return MidiMessage::getMidiNoteName(pitch, true, includeOctave, 5);
	}

	JUCE_DECLARE_NON_COPYABLE(MIDIManager)
};


#endif  // MIDIMANAGER_H_INCLUDED
