/*
  ==============================================================================

    MIDIDeviceParameter.h
    Created: 20 Dec 2016 3:05:54pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

class MIDIDeviceParameterUI;

class MIDIDeviceParameter :
	public Parameter,
	public MIDIManager::Listener
{
public:
	MIDIDeviceParameter(const String &name, const String& description = "MIDI Devices to connect to");
	~MIDIDeviceParameter();

	MIDIInputDevice * inputDevice;
	MIDIOutputDevice * outputDevice;
	
	String ghostDeviceIn;
	String ghostDeviceOut;
	String ghostDeviceNameIn;
	String ghostDeviceNameOut;

	void setInputDevice(MIDIInputDevice * i);
	void setOutputDevice(MIDIOutputDevice * o);

	// Inherited via Listener
	virtual void midiDeviceInAdded(MIDIInputDevice *) override;
	virtual void midiDeviceOutAdded(MIDIOutputDevice *) override;
	virtual void midiDeviceInRemoved(MIDIInputDevice *) override;
	virtual void midiDeviceOutRemoved(MIDIOutputDevice *) override;

	MIDIDeviceParameterUI* createMIDIParameterUI(Array<MIDIDeviceParameter*> parameters = {});
	ControllableUI * createDefaultUI(Array<Controllable*> controllables = {}) override;

	void loadJSONDataInternal(var data) override;

	String getTypeString() const override { return "MIDIDevice"; }

};