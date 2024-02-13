/*
  ==============================================================================

	MIDIModule.h
	Created: 20 Dec 2016 12:35:26pm
	Author:  Ben

  ==============================================================================
*/

#pragma once

class MIDIValueComparator :
	public ControllableComparator
{
public:
	MIDIValueComparator() {}
	int compareElements(Controllable* c1, Controllable* c2) override;
};

class MIDIValueParameter :
	public IntParameter
{
public:
	enum Type { NOTE_ON, NOTE_OFF, CONTROL_CHANGE, PROGRAM_CHANGE, SYSEX, PITCH_WHEEL, CHANNEL_PRESSURE, AFTER_TOUCH, TYPE_MAX };

	MIDIValueParameter(const String& name, const String& description, int value, int channel, int pitchOrNumber, Type t) :
		IntParameter(name, description, value, 0, t == PITCH_WHEEL ? 16383 : 127),
		type(t),
		channel(channel),
		pitchOrNumber(pitchOrNumber)
	{}

	~MIDIValueParameter() {}

	var getJSONDataInternal() override
	{
		var data = IntParameter::getJSONDataInternal();
		data.getDynamicObject()->setProperty("channel", channel);
		data.getDynamicObject()->setProperty("pitchOrNumber", pitchOrNumber);
		data.getDynamicObject()->setProperty("MIDIType", type);
		return data;
	}

	void loadJSONDataInternal(var data) override
	{
		Parameter::loadJSONDataInternal(data);
		channel = data.getProperty("channel", false);
		pitchOrNumber = data.getProperty("pitchOrNumber", false);
		type = static_cast<Type>((int)data.getProperty("MIDIType", false));
	}

	Type type;
	int channel;
	int pitchOrNumber;

	static MIDIValueParameter* create() { return new MIDIValueParameter("New MIDI Value Parameter", "", 0, 0, 0, NOTE_ON); }
	virtual String getTypeString() const override { return getTypeStringStatic(); }
	static String getTypeStringStatic() { return "MIDI Value"; }
};

class MIDIModule :
	public Module,
	public MIDIInputDevice::MIDIInputListener,
	public MTCReceiver::MTCListener
{
public:
	MIDIModule(const String& name = "MIDI", bool useGenericControls = true);
	virtual ~MIDIModule();

	MIDIDeviceParameter* midiParam;
	BoolParameter* autoAdd;
	BoolParameter* autoFeedback;
	BoolParameter* useHierarchy;
	IntParameter* octaveShift;
	BoolParameter* usePitchForNoteNames;

	bool manualAddMode; //to allow manual add override

	MIDIInputDevice* inputDevice;
	MIDIOutputDevice* outputDevice;

	ControllableContainer tempoCC;
	Trigger* midiStartTrigger;
	Trigger* midiStopTrigger;
	Trigger* midiContinueTrigger;

	FloatParameter* bpm;
	BoolParameter* sendClock;
	MIDIClockSender outClock;
	double lastClockReceiveTime;
	double clockDeltaTimes[24];
	int lastClockReceiveTimeIndex;

	ControllableContainer mtcCC;
	FloatParameter* mtcTime;
	BoolParameter* mtcIsPlaying;
	std::unique_ptr<MTCReceiver> mtcReceiver;

	ControllableContainer infoCC;
	Trigger* notePlayed;
	BoolParameter* oneNoteOn;
	IntParameter* lastChannel;
	IntParameter* lastPitch;
	IntParameter* lastVelocity;

	Array<int> noteOns;

	BoolParameter* isConnected;

	std::unique_ptr<ControllableContainer> thruManager;

	static MIDIValueComparator midiValueComparator;

	//Script
	const Identifier noteOnEventId = "noteOnEvent";
	const Identifier noteOffEventId = "noteOffEvent";
	const Identifier ccEventId = "ccEvent";
	const Identifier sysexEventId = "sysExEvent";
	const Identifier pitchWheelEventId = "pitchWheelEvent";
	const Identifier channelPressureId = "channelPressureEvent";
	const Identifier programChangeId = "programChangeEvent";
	const Identifier afterTouchId = "afterTouchEvent";
	const Identifier machineControlCommandId = "machineControlCommandEvent";
	const Identifier machineControlGotoId = "machineControlGotoEvent";

	const Identifier sendNoteOnId = "sendNoteOn";
	const Identifier sendNoteOffId = "sendNoteOff";
	const Identifier sendCCId = "sendCC";
	const Identifier sendSysexId = "sendSysex";
	const Identifier sendProgramChangeId = "sendProgramChange";
	const Identifier sendPitchWheelId = "sendPitchWheel";
	const Identifier sendChannelPressureId = "sendChannelPressure";
	const Identifier sendAfterTouchId = "sendAfterTouch";
	const Identifier sendMachineControlCommandId = "sendMachineControlCommand";
	const Identifier sendMachineControlGotoId = "sendMachineControlGoto";

	bool useGenericControls;

	virtual void sendNoteOn(int channel, int pitch, int velocity);
	virtual void sendNoteOff(int channel, int pitch);
	virtual void sendControlChange(int channel, int number, int value);
	virtual void sendSysex(Array<uint8> data);
	virtual void sendProgramChange(int channel, int number);
	virtual void sendPitchWheel(int channel, int value);
	virtual void sendChannelPressure(int channel, int value);
	virtual void sendAfterTouch(int channel, int note, int value);
	virtual void sendFullFrameTimecode(int hours, int minutes, int seconds, int frames, MidiMessage::SmpteTimecodeType timecodeType);

	void sendMidiMachineControlCommand(MidiMessage::MidiMachineControlCommand command);
	void sendMidiMachineControlGoto(int hours, int minutes, int seconds, int frames);

	void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;
	virtual void onContainerParameterChangedInternal(Parameter* p) override;
	void updateMIDIDevices();

	virtual void noteOnReceived(const int& channel, const int& pitch, const int& velocity) override;
	virtual void noteOffReceived(const int& channel, const int& pitch, const int& velocity) override;
	virtual void controlChangeReceived(const int& channel, const int& number, const int& value) override;
	virtual void programChangeReceived(const int& channel, const int& value) override;
	virtual void sysExReceived(const MidiMessage& msg) override;
	virtual void fullFrameTimecodeReceived(const MidiMessage& msg) override;
	virtual void pitchWheelReceived(const int& channel, const int& value) override;
	virtual void channelPressureReceived(const int& channel, const int& value) override;
	virtual void afterTouchReceived(const int& channel, const int& note, const int& value) override;

	virtual void midiMessageReceived(const MidiMessage& msg) override;

	virtual void midiClockReceived() override;
	virtual void midiStartReceived() override;
	virtual void midiStopReceived() override;
	virtual void midiContinueReceived() override;

	virtual void midiMachineControlCommandReceived(const MidiMessage::MidiMachineControlCommand& type) override;
	virtual void midiMachineControlGotoReceived(const int& hours, const int& minutes, const int& seconds, const int& frames) override;

	virtual void mtcStarted() override;
	virtual void mtcStopped() override;
	virtual void mtcTimeUpdated(bool isFullFrame) override;


	//Script
	static var sendNoteOnFromScript(const var::NativeFunctionArgs& args);
	static var sendNoteOffFromScript(const var::NativeFunctionArgs& args);
	static var sendCCFromScript(const var::NativeFunctionArgs& args);
	static var sendSysexFromScript(const var::NativeFunctionArgs& args);
	static var sendProgramChangeFromScript(const var::NativeFunctionArgs& args);
	static var sendPitchWheelFromScript(const var::NativeFunctionArgs& args);
	static var sendChannelPressureFromScript(const var::NativeFunctionArgs& args);
	static var sendAfterTouchFromScript(const var::NativeFunctionArgs& args);
	static var sendMidiMachineControlCommandFromScript(const var::NativeFunctionArgs& args);
	static var sendMidiMachineControlGotoFromScript(const var::NativeFunctionArgs& args);

	void updateValue(const int& channel, const String& n, const int& val, const MIDIValueParameter::Type& type, const int& pitchOrNumber);

	static void showMenuAndCreateValue(ControllableContainer* container);
	static void createThruControllable(ControllableContainer* cc);

	//Routing
	class MIDIRouteParams :
		public RouteParams
	{
	public:
		MIDIRouteParams(Module* sourceModule, Controllable* c);
		~MIDIRouteParams() {}
		EnumParameter* type;
		IntParameter* channel;
		IntParameter* pitchOrNumber;

		virtual void onContainerParameterChanged(Parameter* p) override;

	};

	virtual RouteParams* createRouteParamsForSourceValue(Module* sourceModule, Controllable* c, int /*index*/) override { return new MIDIRouteParams(sourceModule, c); }
	virtual void handleRoutedModuleValue(Controllable* c, RouteParams* p) override;

	class MIDIModuleRouterController :
		public ModuleRouterController
	{
	public:
		MIDIModuleRouterController(ModuleRouter* router);

		Trigger* setAllCC;
		Trigger* setAllNote;
		Trigger* autoSetPitch;

		void triggerTriggered(Trigger* t) override;
	};

	ModuleRouterController* createModuleRouterController(ModuleRouter* router) override { return new MIDIModuleRouterController(router); }


	void loadJSONDataInternal(var data) override;

	static MIDIModule* create() { return new MIDIModule(); }
	virtual String getDefaultTypeString() const override { return "MIDI"; }

	//InspectableEditor * getEditor(bool isRoot) override;
};
