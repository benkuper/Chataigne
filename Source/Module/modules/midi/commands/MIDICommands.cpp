/*
  ==============================================================================

	MIDICommands.cpp
	Created: 20 Dec 2016 3:45:21pm
	Author:  Ben

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

MIDICommand::MIDICommand(MIDIModule* module, CommandContext context, var params, Multiplex* multiplex) :
	BaseCommand(module, context, params, multiplex),
	midiModule(module)
{

}

MIDICommand::~MIDICommand()
{
}

MIDINoteAndCCCommand::MIDINoteAndCCCommand(MIDIModule* module, CommandContext context, var params, Multiplex* multiplex) :
	MIDICommand(module, context, params, multiplex),
	velocity(nullptr),
	onTime(nullptr),
	remap01To127(nullptr),
	noteEnum(nullptr),
	octave(nullptr),
	noteMode(nullptr),
	number(nullptr),
	maxRemap(127),
	fullNoteChan(0),
	fullNotePitch(0)
{
	channel = addIntParameter("Channel", "Channel for the note message", 1, 1, 16);
	type = (MessageType)(int)params.getProperty("type", 0);

	switch (type)
	{
	case NOTE_ON:
	case NOTE_OFF:
	case FULL_NOTE:
	case AFTER_TOUCH:
		noteMode = new EnumParameter("Note Mode", "How to assign the note");
		addParameter(noteMode, controllables.indexOf(channel));
		noteMode->addOption("Note", MODE_NOTE)->addOption("Pitch", MODE_PITCH);
		updateNoteParams();
		if (type != NOTE_OFF) velocity = addIntParameter("Velocity", "Velocity of the note, between 0 and 127", 127, 0, 127);
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
		remap01To127 = addBoolParameter("Remap to 0-" + String(maxRemap), "If checked, this will automatically remap values from 0-1 to 0-" + String(maxRemap), false);
	}

	if (velocity != nullptr) linkParamToMappingIndex(velocity, 0);
}

MIDINoteAndCCCommand::~MIDINoteAndCCCommand()
{

}

void MIDINoteAndCCCommand::updateNoteParams()
{
	NoteMode m = noteMode->getValueDataAsEnum<NoteMode>();

	if (m == MODE_NOTE)
	{
		noteEnum = new EnumParameter("Note", "Note from C to B");
		for (int i = 0; i < 12; ++i)
		{
			noteEnum->addOption(MidiMessage::getMidiNoteName(i, true, false, 5), i);
		}
		octave = new IntParameter("Octave", "Octave for the note", 0, -10, 10);

		if(number != nullptr) removeControllable(number);
		number = nullptr;

		addParameter(noteEnum, controllables.indexOf(channel) + 1);
		addParameter(octave, controllables.indexOf(noteEnum) + 1);
	}
	else
	{
		int pitch = getPitchFromNote(0);
		number = new IntParameter("Pitch", "The pitch of the note", 0, 0, 127);
		number->setValue(pitch);

		removeControllable(noteEnum);
		removeControllable(octave);
		noteEnum = nullptr;
		octave = nullptr;

		addParameter(number, controllables.indexOf(channel) + 1);
	}


}

void MIDINoteAndCCCommand::setValue(var value, int multiplexIndex)
{
	var newVal;
	float mapFactor = (remap01To127 != nullptr && remap01To127->boolValue()) ? maxRemap : 1;

	if (value.isArray()) for (int i = 0; i < value.size(); i++) newVal.append((float)value[i] * mapFactor);
	else newVal = (float)value * mapFactor;

	MIDICommand::setValue(newVal, multiplexIndex);
}

int MIDINoteAndCCCommand::getPitchFromNote(int multiplexIndex)
{
	if (octave == nullptr || noteEnum == nullptr || moduleRef.wasObjectDeleted()) return 0;

	int octavePitch = ((int)getLinkedValue(octave, multiplexIndex) + midiModule->octaveShift->intValue() + 2) * 12;
	return (int)getLinkedValue(noteEnum, multiplexIndex) + octavePitch;
}

void MIDINoteAndCCCommand::triggerInternal(int multiplexIndex)
{
	MIDICommand::triggerInternal(multiplexIndex);

	int pitch = 0;
	if (type == CONTROLCHANGE || type == PROGRAMCHANGE) pitch = getLinkedValue(number, multiplexIndex);
	else if (type == NOTE_ON || type == NOTE_OFF || type == FULL_NOTE || type == AFTER_TOUCH)
	{
		NoteMode m = noteMode->getValueDataAsEnum<NoteMode>();
		if (m == MODE_PITCH) pitch = getLinkedValue(number, multiplexIndex);
		else pitch = getPitchFromNote(multiplexIndex);
	}

	int chanVal = channel != nullptr ? (int)getLinkedValue(channel, multiplexIndex) : 0;
	int velVal = velocity != nullptr ? (int)getLinkedValue(velocity, multiplexIndex) : 0;

	switch (type)
	{
	case NOTE_ON:
		midiModule->sendNoteOn(chanVal, pitch, velVal);
		break;

	case NOTE_OFF:
		midiModule->sendNoteOff(chanVal, pitch);
		break;

	case FULL_NOTE:
		midiModule->sendNoteOn(chanVal, pitch, velVal);
		fullNoteChan = chanVal;
		fullNotePitch = pitch;
		startTimer(onTime->floatValue() * 1000);
		break;

	case CONTROLCHANGE:
		midiModule->sendControlChange(chanVal, pitch, velVal);
		break;

	case PROGRAMCHANGE:
		midiModule->sendProgramChange(chanVal, pitch);
		break;

	case PITCH_WHEEL:
		midiModule->sendPitchWheel(chanVal, velVal);
		break;

	case CHANNEL_PRESSURE:
		midiModule->sendChannelPressure(chanVal, velVal);
		break;

	case AFTER_TOUCH:
		midiModule->sendAfterTouch(chanVal, pitch, velVal);
		break;

	default:
		DBG("NOT A VALID TYPE !");
		break;
	}
}

void MIDINoteAndCCCommand::onContainerParameterChanged(Parameter* p)
{
	MIDICommand::onContainerParameterChanged(p);
	if (p == noteMode)
	{
		updateNoteParams();
	}
}

void MIDINoteAndCCCommand::hiResTimerCallback()
{
	stopTimer();
	if (!moduleRef.wasObjectDeleted()) midiModule->sendNoteOff(fullNoteChan, fullNotePitch);
}


MIDISysExCommand::MIDISysExCommand(MIDIModule* module, CommandContext context, var params, Multiplex* multiplex) :
	MIDICommand(module, context, params, multiplex),
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
		IntParameter* p = new IntParameter("#" + index, "Data for the byte #" + index, 0, 0, 255);
		p->hexMode = true;
		dataContainer.addParameter(p); // after hexMode
	}

}

void MIDISysExCommand::onContainerParameterChanged(Parameter* p)
{
	MIDICommand::onContainerParameterChanged(p);
	if (p == numBytes)
	{
		updateBytesParams();
	}
}

void MIDISysExCommand::triggerInternal(int multiplexIndex)
{
	MIDICommand::triggerInternal(multiplexIndex);

	Array<uint8> data;
	for (auto& c : dataContainer.controllables) data.add(((IntParameter*)c)->intValue());
	midiModule->sendSysex(data);
}

MIDIMidiMachineControlCommandAndGoto::MIDIMidiMachineControlCommandAndGoto(MIDIModule* module, CommandContext context, var params, Multiplex* multiplex) :
	MIDICommand(module, context, params, multiplex)
{
	isGoto = (bool)params.getProperty("goto", 0);

	if (isGoto)
	{
		time = addFloatParameter("Time", "Time to jump to", 0, 0);
		time->defaultUI = FloatParameter::TIME;
	}
	else
	{
		command = addEnumParameter("Command", "Command to send");
		command->addOption("Stop", MidiMessage::mmc_stop)->addOption("Play", MidiMessage::mmc_play)->addOption("Deferred play", MidiMessage::mmc_deferredplay)->addOption("Fast forward", MidiMessage::mmc_fastforward)->addOption("Rewind", MidiMessage::mmc_rewind)->addOption("Record start", MidiMessage::mmc_recordStart)->addOption("Record stop", MidiMessage::mmc_recordStop)->addOption("Pause", MidiMessage::mmc_pause);
	}
}
MIDIMidiMachineControlCommandAndGoto::~MIDIMidiMachineControlCommandAndGoto()
{
}


void MIDIMidiMachineControlCommandAndGoto::triggerInternal(int multiplexIndex)
{
	MIDICommand::triggerInternal(multiplexIndex);

	if (isGoto)
	{
		double unused;
		float position = getLinkedValue(time, multiplexIndex);
		int m_frame = static_cast<int>(modf(position, &unused) * fps);
		int m_second = static_cast<int>(position) % 60;
		int m_minute = (static_cast<int>(position) / 60) % 60;
		int m_hour = (static_cast<int>(position) / 60 / 60) % 60;

		midiModule->sendMidiMachineControlGoto(m_hour, m_minute, m_second, m_frame);
	}
	else
	{
		MidiMessage::MidiMachineControlCommand commandVal = (MidiMessage::MidiMachineControlCommand)(int)getLinkedValue(command, multiplexIndex);

		midiModule->sendMidiMachineControlCommand(commandVal);
	}
}
