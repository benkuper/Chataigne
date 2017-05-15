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
	MIDICommand(module,context,params)
{
	channel = addIntParameter("Channel", "Channel for the note message", 1, 1, 16);
	type = (MessageType)(int)params.getProperty("type", 0);
	switch (type)
	{
	case NOTE_ON:
	case NOTE_OFF:
		noteEnum = addEnumParameter("Note", "Note from C to B");
		for (int i = 0; i < 12; i++)
		{
			noteEnum->addOption(MidiMessage::getMidiNoteName(i, true, false, 5), i);
		}
		octave = addIntParameter("Octave", "Octave for the note", 5, 0, 10);
		velocity = addIntParameter("Velocity", "Velocity of the note, between 0 and 127", type == NOTE_OFF ? 0 : 127, 0, 127);
		break;

	case CONTROLCHANGE:
		number = addIntParameter("CC Number", "Number of the CC (0-127)", 0, 0, 127);
		velocity = addIntParameter("CC Value", "Value of the CC", 0, 0, 127);
	}

	setTargetMappingParameterAt(velocity, 0);
}

MIDINoteAndCCCommand::~MIDINoteAndCCCommand()
{

}

void MIDINoteAndCCCommand::setValue(var value)
{
	BaseCommand::setValue(value);
	velocity->setValue(value, true);		
	trigger();
}

void MIDINoteAndCCCommand::trigger()
{
	DBG("MIDI Command trigger");
	int pitch = type == CONTROLCHANGE? number->intValue() : (int)noteEnum->getValueData() + (octave->intValue() - (int)octave->minimumValue) * 12;

	switch(type)
	{
	case NOTE_ON:
		midiModule->sendNoteOn(pitch, velocity->intValue(), channel->intValue());
		break;

	case NOTE_OFF:
		midiModule->sendNoteOff(pitch, channel->intValue());
		break;

	case CONTROLCHANGE:
		midiModule->sendControlChange(pitch, velocity->intValue(), channel->intValue());
		break;

	default:
		DBG("NOT A VALID TYPE !");
		break;
	}
}
