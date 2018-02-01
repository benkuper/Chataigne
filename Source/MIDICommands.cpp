/*
  ==============================================================================

    MIDICommands.cpp
    Created: 20 Dec 2016 3:45:21pm
    Author:  Ben

  ==============================================================================
*/

#include "MIDICommands.h"
#include "MIDIModule.h"

MIDICommand::MIDICommand(MIDIModule * module, CommandContext context, var params) :
	BaseCommand(module,context,params),
	midiModule(module)
{
	
}

MIDICommand::~MIDICommand()
{
}

MIDINoteAndCCCommand::MIDINoteAndCCCommand(MIDIModule * module, CommandContext context, var params) :
	MIDICommand(module, context, params),
	velocity(nullptr),
	onTime(nullptr)
{
	channel = addIntParameter("Channel", "Channel for the note message", 1, 1, 16);
	type = (MessageType)(int)params.getProperty("type", 0);
	switch (type)
	{
	case NOTE_ON:
	case NOTE_OFF:
	case FULL_NOTE:
		noteEnum = addEnumParameter("Note", "Note from C to B");
		for (int i = 0; i < 12; i++)
		{
			noteEnum->addOption(MidiMessage::getMidiNoteName(i, true, false, 5), i);
		}
		octave = addIntParameter("Octave", "Octave for the note", 5, 0, 10);
		if(type != NOTE_OFF) velocity = addIntParameter("Velocity", "Velocity of the note, between 0 and 127", 127, 0, 127);
		break;

	case CONTROLCHANGE:
		number = addIntParameter("CC Number", "Number of the CC (0-127)", 0, 0, 127);
		velocity = addIntParameter("CC Value", "Value of the CC", 0, 0, 127);
	}

	if (type == FULL_NOTE)
	{
		onTime = addFloatParameter("On Time", "Interval between the note on and note off", 1, 0, 3600);
		onTime->defaultUI = FloatParameter::TIME;
	}

	setTargetMappingParameterAt(velocity, 0);
}

MIDINoteAndCCCommand::~MIDINoteAndCCCommand()
{

}

void MIDINoteAndCCCommand::setValue(var value)
{
	BaseCommand::setValue(value);
	if(velocity != nullptr) velocity->setValue(value, true);		
	trigger();
}

void MIDINoteAndCCCommand::trigger()
{
	MIDICommand::trigger();

	int pitch = type == CONTROLCHANGE? number->intValue() : (int)noteEnum->getValueData() + (octave->intValue() - (int)octave->minimumValue) * 12;

	switch(type)
	{
	case NOTE_ON:
		midiModule->sendNoteOn(pitch, velocity->intValue(), channel->intValue());
		break;

	case NOTE_OFF:
		midiModule->sendNoteOff(pitch, channel->intValue());
		break;

	case FULL_NOTE:
		midiModule->sendNoteOn(pitch, velocity->intValue(), channel->intValue());
		startTimer(onTime->floatValue() * 1000);
		break;

	case CONTROLCHANGE:
		midiModule->sendControlChange(pitch, velocity->intValue(), channel->intValue());
		break;

	default:
		DBG("NOT A VALID TYPE !");
		break;
	}
}

void MIDINoteAndCCCommand::timerCallback()
{
	stopTimer();
	int pitch = (int)noteEnum->getValueData() + (octave->intValue() - (int)octave->minimumValue) * 12;
	if (midiModule != nullptr) midiModule->sendNoteOff(pitch, channel->intValue());
}


MIDISysExCommand::MIDISysExCommand(MIDIModule * module, CommandContext context, var params) :
	MIDICommand(module, context, params),
	dataContainer("Bytes")
{
	numBytes = addIntParameter("Num bytes", "Number of bytes to send", 1, 1, 512);
	addChildControllableContainer(&dataContainer);
	updateBytesParams();
}
MIDISysExCommand::~MIDISysExCommand()
{
}

void MIDISysExCommand::updateBytesParams()
{

	while (dataContainer.controllables.size() > numBytes->intValue())
	{
		dataContainer.controllables[dataContainer.controllables.size() - 1]->remove();
	}

	while (dataContainer.controllables.size() < numBytes->intValue())
	{
		String index = String(dataContainer.controllables.size());
		dataContainer.addIntParameter("#" + index, "Data for the byte #" + index, 0, 0, 255);
	}

}

void MIDISysExCommand::onContainerParameterChangedAsync(Parameter * p, const var &)
{
	if (p == numBytes)
	{
		updateBytesParams();
	}
}

void MIDISysExCommand::trigger()
{
	MIDICommand::trigger();

	Array<uint8> data;
	for (auto &c : dataContainer.controllables) data.add(((IntParameter *)c)->intValue());
	midiModule->sendSysex(data);
}

