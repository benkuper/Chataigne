/*
  ==============================================================================

    MIDICommands.h
    Created: 20 Dec 2016 3:45:21pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

class MIDIModule;

class MIDICommand :
	public BaseCommand
{
public:
	MIDICommand(MIDIModule * module, CommandContext context, var params, Multiplex * multiplex = nullptr);
	virtual ~MIDICommand();

	MIDIModule *  midiModule;

	static BaseCommand * create(ControllableContainer * module, CommandContext context, var params, Multiplex * multiplex) { return new MIDICommand((MIDIModule *)module, context, params, multiplex); }
};


class MIDINoteAndCCCommand :
	public MIDICommand,
	public HighResolutionTimer
{
public:
	MIDINoteAndCCCommand(MIDIModule * module, CommandContext context, var params, Multiplex * multiplex = nullptr);
	~MIDINoteAndCCCommand();

	enum MessageType {NOTE_ON,NOTE_OFF,FULL_NOTE, CONTROLCHANGE, PROGRAMCHANGE, PITCH_WHEEL, CHANNEL_PRESSURE, AFTER_TOUCH};
	enum MapTo { PITCH, VELOCITY };
	MessageType type;

	IntParameter * channel;
	EnumParameter * noteEnum;
	IntParameter * octave;
	IntParameter * number; //for CC
	IntParameter * velocity;
	FloatParameter * onTime;

	BoolParameter * remap01To127;
	int maxRemap;

	void setValue(var value, int multiplexIndex) override;
	void triggerInternal(int multiplexIndex) override;

	virtual void hiResTimerCallback() override;


	static MIDINoteAndCCCommand * create(ControllableContainer * module, CommandContext context, var params, Multiplex * multiplex) { return new MIDINoteAndCCCommand((MIDIModule *)module, context, params, multiplex); }

};

class MIDISysExCommand :
	public MIDICommand
{
public:
	MIDISysExCommand(MIDIModule * module, CommandContext context, var params, Multiplex * multiplex = nullptr);
	~MIDISysExCommand();

	IntParameter * numBytes;
	ControllableContainer dataContainer;
	Array<IntParameter *> bytes;

	void updateBytesParams();
	void onContainerParameterChangedAsync(Parameter * p, const var &param) override;
	void triggerInternal(int multiplexIndex) override;

	static MIDISysExCommand * create(ControllableContainer * module, CommandContext context, var params, Multiplex * multiplex) { return new MIDISysExCommand((MIDIModule *)module, context, params, multiplex); }

};