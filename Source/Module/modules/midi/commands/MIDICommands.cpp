/*
  ==============================================================================

    MIDICommands.cpp
    Created: 20 Dec 2016 3:45:21pm
    Author:  Ben

  ==============================================================================
*/

#include "MIDICommands.h"
#include "../MIDIModule.h"

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
	onTime(nullptr),
	remap01To127(nullptr),
	maxRemap(127)
{
	channel = addIntParameter("Channel", "Channel for the note message", 1, 1, 16);
	type = (MessageType)(int)params.getProperty("type", 0);

	switch (type)
	{
	case NOTE_ON:
	case NOTE_OFF:
	case FULL_NOTE:
	case AFTER_TOUCH:
		noteEnum = addEnumParameter("Note", "Note from C to B");
		for (int i = 0; i < 12; ++i)
		{
			noteEnum->addOption(MidiMessage::getMidiNoteName(i, true, false, 5), i);
		}
		octave = addIntParameter("Octave", "Octave for the note", 5, 0, 10);
		if(type != NOTE_OFF) velocity = addIntParameter("Velocity", "Velocity of the note, between 0 and 127", 127, 0, 127);
		break;

	case CONTROLCHANGE:
		number = addIntParameter("CC Number", "Number of the CC (0-127)", 0, 0, 127);
		velocity = addIntParameter("CC Value", "Value of the CC", 0, 0, 127);
		break;

	case PROGRAMCHANGE:
		number = addIntParameter("Program", "The program to set", 0, 0, 127);
		break;

	case PITCH_WHEEL:
		velocity = addIntParameter("Value", "The value of the pitchWheel", 0, 0, 16383);
		maxRemap = velocity->maximumValue;
		break;

	case CHANNEL_PRESSURE:
		velocity = addIntParameter("Value", "The value of the channel pressure", 0, 0, 127);
		break;
	}

	if (type == FULL_NOTE)
	{
		onTime = addFloatParameter("On Time", "Interval between the note on and note off", 1, 0, 3600);
		onTime->defaultUI = FloatParameter::TIME;
	}

	if (context == MAPPING)
	{
		remap01To127 = addBoolParameter("Remap to 0-"+String(maxRemap), "If checked, this will automatically remap values from 0-1 to 0-"+String(maxRemap), false);
	}

	if(velocity != nullptr) addTargetMappingParameterAt(velocity, 0);
}

MIDINoteAndCCCommand::~MIDINoteAndCCCommand()
{

}

void MIDINoteAndCCCommand::setValue(var value)
{
	float mapFactor = (remap01To127 != nullptr && remap01To127->boolValue()) ? maxRemap : 1;
	if (value.isArray()) value[0] = (float)value[0] * mapFactor;
	else value = (float)value * mapFactor;
	
	MIDICommand::setValue(value);
}

void MIDINoteAndCCCommand::triggerInternal()
{
	MIDICommand::triggerInternal();

	int pitch = 0;
	if (type == CONTROLCHANGE || type == PROGRAMCHANGE) pitch = number->intValue();
	else if(type == NOTE_ON || type == NOTE_OFF || type == FULL_NOTE || type == AFTER_TOUCH) pitch = (int)noteEnum->getValueData() + (octave->intValue() - (int)octave->minimumValue) * 12;

	switch(type)
	{
	case NOTE_ON:
		midiModule->sendNoteOn(channel->intValue(), pitch, velocity->intValue());
		break;

	case NOTE_OFF:
		midiModule->sendNoteOff(channel->intValue(), pitch);
		break;

	case FULL_NOTE:
		midiModule->sendNoteOn(channel->intValue(), pitch, velocity->intValue());
		startTimer(onTime->floatValue() * 1000);
		break;

	case CONTROLCHANGE:
		midiModule->sendControlChange(channel->intValue(), pitch, velocity->intValue());
		break;

	case PROGRAMCHANGE:
		midiModule->sendProgramChange(channel->intValue(), pitch);
		break;

	case PITCH_WHEEL:
		midiModule->sendPitchWheel(channel->intValue(), velocity->intValue());
		break;

	case CHANNEL_PRESSURE:
		midiModule->sendChannelPressure(channel->intValue(), velocity->intValue());
		break;

	case AFTER_TOUCH:
		midiModule->sendAfterTouch(channel->intValue(), pitch, velocity->intValue());
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
	if (midiModule != nullptr) midiModule->sendNoteOff(channel->intValue(), pitch);
}


MIDISysExCommand::MIDISysExCommand(MIDIModule * module, CommandContext context, var params) :
	MIDICommand(module, context, params),
	dataContainer("Bytes")
{
	saveAndLoadRecursiveData = true;
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
		IntParameter * p = dataContainer.addIntParameter("#" + index, "Data for the byte #" + index, 0, 0, 255);
		p->hexMode = true;
	}

}

void MIDISysExCommand::onContainerParameterChangedAsync(Parameter * p, const var &)
{
	if (p == numBytes)
	{
		updateBytesParams();
	}
}

void MIDISysExCommand::triggerInternal()
{
	MIDICommand::triggerInternal();

	Array<uint8> data;
	for (auto &c : dataContainer.controllables) data.add(((IntParameter *)c)->intValue());
	midiModule->sendSysex(data);
}

