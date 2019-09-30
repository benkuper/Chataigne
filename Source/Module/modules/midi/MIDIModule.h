/*
  ==============================================================================

    MIDIModule.h
    Created: 20 Dec 2016 12:35:26pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "Module/Module.h"
#include "Common/MIDI/MIDIManager.h"
#include "Common/MIDI/MIDIDeviceParameter.h"

class MIDIValueParameter :
	public IntParameter
{
public:
	enum Type { NOTE_ON, NOTE_OFF, CONTROL_CHANGE, SYSEX };

	MIDIValueParameter(const String &name, const String &description, int value, int channel, int pitchOrNumber, Type t) :
		IntParameter(name, description, value, 0, 127),
		type(t),
		channel(channel),
		pitchOrNumber(pitchOrNumber)
	{}

	~MIDIValueParameter() {}

	Type type;
	int channel;
	int pitchOrNumber;
};

class MIDIModule :
	public Module,
	public MIDIInputDevice::MIDIInputListener
{
public:
	MIDIModule(const String &name = "MIDI", bool useGenericControls = true);
	virtual ~MIDIModule();

	MIDIDeviceParameter * midiParam;
	BoolParameter * autoAdd;
	BoolParameter * autoFeedback;

	MIDIInputDevice * inputDevice;
	MIDIOutputDevice * outputDevice;

	BoolParameter * isConnected;

	//Script
	const Identifier noteOnEventId = "noteOnEvent";
	const Identifier noteOffEventId = "noteOffEvent";
	const Identifier ccEventId = "ccEvent";
	const Identifier sysexEventId = "sysExEvent";

	const Identifier sendNoteOnId = "sendNoteOn";
	const Identifier sendNoteOffId = "sendNoteOff";
	const Identifier sendCCId = "sendCC";
	const Identifier sendSysexId = "sendSysex";
	const Identifier sendProgramChangeId = "sendProgramChange";
	
	bool useGenericControls;

	virtual void sendNoteOn(int channel, int pitch, int velocity);
	virtual void sendNoteOff(int channel, int pitch);
	virtual void sendControlChange(int channel, int number, int value);
	virtual void sendSysex(Array<uint8> data);
	virtual void sendProgramChange(int channel, int number);
	virtual void sendFullFrameTimecode(int hours, int minutes, int seconds, int frames, MidiMessage::SmpteTimecodeType timecodeType);
	void sendMidiMachineControlCommand(MidiMessage::MidiMachineControlCommand command);

	void onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c) override;
	void updateMIDIDevices();

	virtual void noteOnReceived(const int &channel, const int &pitch, const int &velocity) override;
	virtual void noteOffReceived(const int& channel, const int& pitch, const int& velocity) override;
	virtual void controlChangeReceived(const int &channel, const int &number, const int &value) override;
	virtual void sysExReceived(const MidiMessage & msg) override;
	virtual void fullFrameTimecodeReceived(const MidiMessage& msg) override;

	//Script
	static var sendNoteOnFromScript(const var::NativeFunctionArgs &args);
	static var sendNoteOffFromScript(const var::NativeFunctionArgs &args);
	static var sendCCFromScript(const var::NativeFunctionArgs &args);
	static var sendSysexFromScript(const var::NativeFunctionArgs& args);
	static var sendProgramChangeFromScript(const var::NativeFunctionArgs &args);

	void updateValue(const int &channel, const String &n, const int &val, const MIDIValueParameter::Type &type, const int &pitchOrNumber);

	static void showMenuAndCreateValue(ControllableContainer * container);
	
	//Routing
	class MIDIRouteParams :
		public RouteParams
	{
	public:
		MIDIRouteParams(Module * sourceModule, Controllable * c);
		~MIDIRouteParams() {}
		EnumParameter * type;
		IntParameter * channel;
		IntParameter * pitchOrNumber;
	};

	virtual RouteParams * createRouteParamsForSourceValue(Module * sourceModule, Controllable * c, int /*index*/) override { return new MIDIRouteParams(sourceModule, c); }
	virtual void handleRoutedModuleValue(Controllable * c, RouteParams * p) override;

	void loadJSONDataInternal(var data) override;

	static MIDIModule * create() { return new MIDIModule(); }
	virtual String getDefaultTypeString() const override { return "MIDI"; }

	//InspectableEditor * getEditor(bool isRoot) override;
};