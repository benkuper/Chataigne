/*
  ==============================================================================

    MIDIModule.h
    Created: 20 Dec 2016 12:35:26pm
    Author:  Ben

  ==============================================================================
*/

#ifndef MIDIMODULE_H_INCLUDED
#define MIDIMODULE_H_INCLUDED

#include "Module.h"
#include "MIDIManager.h"
#include "MIDIDeviceParameter.h"

class MIDIModule :
	public Module,
	public MIDIInputDevice::MIDIInputListener
{
public:
	MIDIModule(const String &name = "MIDI");
	virtual ~MIDIModule();

	MIDIDeviceParameter * midiParam;
	BoolParameter * autoAdd;

	MIDIInputDevice * inputDevice;
	MIDIOutputDevice * outputDevice;
	

	void sendNoteOn(int pitch, int velocity, int channel = 0);
	void sendNoteOff(int pitch, int channel = 0);
	void sendControlChange(int number, int value, int channel = 0);

	void onContainerParameterChangedInternal(Parameter *) override;
	void updateMIDIDevices();

	void noteOnReceived(const int &channel, const int &pitch, const int &velocity) override;
	void noteOffReceived(const int &channel, const int &pitch, const int &velocity) override;
	void controlChangeReceived(const int &channel, const int &number, const int &value) override;

	void updateValue(const String &n, const int &val);

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	static MIDIModule * create() { return new MIDIModule(); }
	virtual String getDefaultTypeString() const override { return "MIDI"; }

	InspectableEditor * getEditor(bool isRoot) override;
};



#endif  // MIDIMODULE_H_INCLUDED
