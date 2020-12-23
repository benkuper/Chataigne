/*
  ==============================================================================

    MIDICommands.h
    Created: 20 Dec 2016 3:45:21pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "Common/Command/BaseCommand.h"

class MIDIModule;

class MIDICommand :
	public BaseCommand
{
public:
	MIDICommand(MIDIModule * module, CommandContext context, var params, IteratorProcessor * iterator = nullptr);
	virtual ~MIDICommand();

	MIDIModule *  midiModule;

	static BaseCommand * create(ControllableContainer * module, CommandContext context, var params, IteratorProcessor * iterator) { return new MIDICommand((MIDIModule *)module, context, params, iterator); }
};


class MIDINoteAndCCCommand :
	public MIDICommand,
	public Timer
{
public:
	MIDINoteAndCCCommand(MIDIModule * module, CommandContext context, var params, IteratorProcessor * iterator = nullptr);
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

	void setValue(var value, int iterationIndex) override;
	void triggerInternal(int iterationIndex) override;

	static MIDINoteAndCCCommand * create(ControllableContainer * module, CommandContext context, var params, IteratorProcessor * iterator) { return new MIDINoteAndCCCommand((MIDIModule *)module, context, params, iterator); }


	// Inherited via Timer
	virtual void timerCallback() override;

};

class MIDISysExCommand :
	public MIDICommand
{
public:
	MIDISysExCommand(MIDIModule * module, CommandContext context, var params, IteratorProcessor * iterator = nullptr);
	~MIDISysExCommand();

	IntParameter * numBytes;
	ControllableContainer dataContainer;
	Array<IntParameter *> bytes;

	void updateBytesParams();
	void onContainerParameterChangedAsync(Parameter * p, const var &param) override;
	void triggerInternal(int iterationIndex) override;

	static MIDISysExCommand * create(ControllableContainer * module, CommandContext context, var params, IteratorProcessor * iterator) { return new MIDISysExCommand((MIDIModule *)module, context, params, iterator); }

};