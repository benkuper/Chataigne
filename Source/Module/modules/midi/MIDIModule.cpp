/*
  ==============================================================================

    MIDIModule.cpp
    Created: 20 Dec 2016 12:35:26pm
    Author:  Ben

  ==============================================================================
*/

#include "MIDIModule.h"
#include "commands/MIDICommands.h"
#include "Common/MIDI/MIDIManager.h"
#include "UI/ChataigneAssetManager.h"

MIDIModule::MIDIModule(const String & name, bool _useGenericControls) :
	Module(name),
	inputDevice(nullptr),
	outputDevice(nullptr),
	useGenericControls(_useGenericControls)
{
	setupIOConfiguration(true, true);
	canHandleRouteValues = true;
	includeValuesInSave = true;

	if (useGenericControls)
	{
		autoAdd = moduleParams.addBoolParameter("Auto Add", "Auto Add MIDI values that are received but not in the list", true);
		defManager->add(CommandDefinition::createDef(this, "", "Note On", &MIDINoteAndCCCommand::create)->addParam("type", (int)MIDINoteAndCCCommand::NOTE_ON));
		defManager->add(CommandDefinition::createDef(this, "", "Note Off", &MIDINoteAndCCCommand::create)->addParam("type", (int)MIDINoteAndCCCommand::NOTE_OFF));
		defManager->add(CommandDefinition::createDef(this, "", "Full Note", &MIDINoteAndCCCommand::create)->addParam("type", (int)MIDINoteAndCCCommand::FULL_NOTE));
		defManager->add(CommandDefinition::createDef(this, "", "Controller Change", &MIDINoteAndCCCommand::create)->addParam("type", (int)MIDINoteAndCCCommand::CONTROLCHANGE));
		defManager->add(CommandDefinition::createDef(this, "", "Sysex Message", &MIDISysExCommand::create));
		defManager->add(CommandDefinition::createDef(this, "", "Program Change", &MIDINoteAndCCCommand::create)->addParam("type", (int)MIDINoteAndCCCommand::PROGRAMCHANGE));;
		defManager->add(CommandDefinition::createDef(this, "", "Pitch Wheel", &MIDINoteAndCCCommand::create)->addParam("type", (int)MIDINoteAndCCCommand::PITCH_WHEEL));;
	}
	
	autoFeedback = moduleParams.addBoolParameter("Auto Feedback", "If checked, all changed values will be resent automatically to the outputs", false);

	midiParam = new MIDIDeviceParameter("Devices");
	moduleParams.addParameter(midiParam);

	isConnected = moduleParams.addBoolParameter("Is Connected", "This is checked if the module is connected to at least one input or output device", false);
	isConnected->setControllableFeedbackOnly(true);
	isConnected->isSavable = false; 
	connectionFeedbackRef = isConnected;


	//Script
	scriptObject.setMethod(sendNoteOnId, &MIDIModule::sendNoteOnFromScript);
	scriptObject.setMethod(sendNoteOffId, &MIDIModule::sendNoteOffFromScript);
	scriptObject.setMethod(sendCCId, &MIDIModule::sendCCFromScript);
	scriptObject.setMethod(sendSysexId, &MIDIModule::sendSysexFromScript);
	scriptObject.setMethod(sendProgramChangeId, &MIDIModule::sendProgramChangeFromScript);
	scriptObject.setMethod(sendPitchWheelId, &MIDIModule::sendPitchWheelFromScript);

	scriptManager->scriptTemplate += ChataigneAssetManager::getInstance()->getScriptTemplate("midi");

	valuesCC.userCanAddControllables = true;
	valuesCC.customUserCreateControllableFunc = &MIDIModule::showMenuAndCreateValue;
	alwaysShowValues = true;

	setupIOConfiguration(inputDevice != nullptr, outputDevice != nullptr);
}

MIDIModule::~MIDIModule()
{
	if(inputDevice != nullptr) inputDevice->removeMIDIInputListener(this);
	if (outputDevice != nullptr) outputDevice->close();
}


void MIDIModule::sendNoteOn(int channel, int pitch, int velocity)
{
	if (!enabled->boolValue()) return;
	if (outputDevice == nullptr) return;
	if (logOutgoingData->boolValue()) NLOG(niceName, "Send Note on " << MIDIManager::getNoteName(pitch) << ", " << velocity << ", " << channel);
	outActivityTrigger->trigger();
	outputDevice->sendNoteOn(channel, pitch, velocity);
}

void MIDIModule::sendNoteOff(int channel, int pitch)
{
	if (!enabled->boolValue()) return;
	if (outputDevice == nullptr) return;
	if (logOutgoingData->boolValue()) NLOG(niceName, "Send Note off " << MIDIManager::getNoteName(pitch) << ", " << channel);
	outActivityTrigger->trigger();
	outputDevice->sendNoteOff(channel, pitch);
}

void MIDIModule::sendControlChange(int channel, int number, int value)
{
	if (!enabled->boolValue()) return;
	if (outputDevice == nullptr) return;
	if (logOutgoingData->boolValue()) NLOG(niceName, "Send Control Change " << number << ", " << value << ", " << channel);
	outActivityTrigger->trigger();
	outputDevice->sendControlChange(channel, number, value);
}

void MIDIModule::sendProgramChange(int channel, int program)
{
	if (!enabled->boolValue()) return;
	if (outputDevice == nullptr) return;
	if (logOutgoingData->boolValue()) NLOG(niceName, "Send ProgramChange " << program);
	outActivityTrigger->trigger();
	outputDevice->sendProgramChange(channel, program);
}

void MIDIModule::sendPitchWheel(int channel, int value)
{
	if (!enabled->boolValue()) return;
	if (outputDevice == nullptr) return;
	if (logOutgoingData->boolValue()) NLOG(niceName, "Send PitchWheel " << channel << ", " << value);
	outputDevice->sendPitchWheel(channel, value);
}

void MIDIModule::sendSysex(Array<uint8> data)
{
	if (!enabled->boolValue()) return;
	if (outputDevice == nullptr) return;
	if (logOutgoingData->boolValue()) NLOG(niceName, "Send Sysex " << data.size() << " bytes");
	outActivityTrigger->trigger();
	outputDevice->sendSysEx(data);
}

void MIDIModule::sendFullFrameTimecode(int hours, int minutes, int seconds, int frames, MidiMessage::SmpteTimecodeType timecodeType)
{
	if (!enabled->boolValue()) return;
	if (outputDevice == nullptr) return;
	if (logOutgoingData->boolValue())
	{
		NLOG(niceName, "Send full frame timecode : " << hours << ":" << minutes << ":" << seconds << "." << frames << " / " << timecodeType);
	}
	outputDevice->sendFullframeTimecode(hours, minutes, seconds, frames, timecodeType);
}

void MIDIModule::sendMidiMachineControlCommand(MidiMessage::MidiMachineControlCommand command)
{
	if (!enabled->boolValue()) return;
	if (outputDevice == nullptr) return;
	if (logOutgoingData->boolValue())
	{
		NLOG(niceName, "Send midi machine control command : " << command);
	}
	outputDevice->sendMidiMachineControlCommand(command);
}


void MIDIModule::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c)
{
	Module::onControllableFeedbackUpdateInternal(cc, c);

	if (c == midiParam)
	{
		updateMIDIDevices();
	}


	if (autoFeedback->boolValue())
	{
		if (isControllableInValuesContainer(c))
		{
			MIDIValueParameter * mvp = dynamic_cast<MIDIValueParameter *>(c);
			if (mvp != nullptr)
			{
				switch (mvp->type)
				{
				case MIDIValueParameter::NOTE_ON: sendNoteOn(mvp->channel, mvp->pitchOrNumber, mvp->intValue()); break;
				case MIDIValueParameter::NOTE_OFF: sendNoteOff(mvp->channel, mvp->pitchOrNumber); break;
				case MIDIValueParameter::CONTROL_CHANGE: sendControlChange(mvp->channel, mvp->pitchOrNumber, mvp->intValue()); break;
                        
                default:
                    break;
				}
			}
		}
	}
}

void MIDIModule::updateMIDIDevices()
{
	MIDIInputDevice * newInput = midiParam->inputDevice;
	//if (inputDevice != newInput)
	//{
		if (inputDevice != nullptr)
		{
			inputDevice->removeMIDIInputListener(this);
		}
		inputDevice = newInput;
		if (inputDevice != nullptr)
		{
			inputDevice->addMIDIInputListener(this);
		}
	//}

	MIDIOutputDevice * newOutput = midiParam->outputDevice;
	//if (outputDevice != newOutput)
	//{
		if(outputDevice != nullptr) outputDevice->close();
		outputDevice = newOutput;
		if(outputDevice != nullptr) outputDevice->open();
	//} 

	setupIOConfiguration(inputDevice != nullptr || valuesCC.controllables.size() > 0, outputDevice != nullptr);

	isConnected->setValue(inputDevice != nullptr || outputDevice != nullptr);
}

void MIDIModule::noteOnReceived(const int & channel, const int & pitch, const int & velocity)
{
	if (!enabled->boolValue()) return; 
	inActivityTrigger->trigger();
	if (logIncomingData->boolValue())  NLOG(niceName, "Note On : " << channel << ", " << MIDIManager::getNoteName(pitch) << ", " << velocity);

	if (useGenericControls) updateValue(channel, MIDIManager::getNoteName(pitch), velocity, MIDIValueParameter::NOTE_ON, pitch);

	if (scriptManager->items.size() > 0) scriptManager->callFunctionOnAllItems(noteOnEventId, Array<var>(channel, pitch, velocity));
}

void MIDIModule::noteOffReceived(const int & channel, const int & pitch, const int & velocity)
{
	if (!enabled->boolValue()) return; 
	inActivityTrigger->trigger();
	if (logIncomingData->boolValue()) NLOG(niceName, "Note Off : " << channel << ", " << MIDIManager::getNoteName(pitch) << ", " << velocity);

	if (useGenericControls) updateValue(channel, MIDIManager::getNoteName(pitch), velocity, MIDIValueParameter::NOTE_OFF, pitch);

	if (scriptManager->items.size() > 0) scriptManager->callFunctionOnAllItems(noteOffEventId, Array<var>(channel, pitch, velocity));
	
}

void MIDIModule::controlChangeReceived(const int & channel, const int & number, const int & value)
{
	if (!enabled->boolValue()) return; 
	inActivityTrigger->trigger();
	if (logIncomingData->boolValue()) NLOG(niceName, "Control Change : " << channel << ", " << number << ", " << value);

	if (useGenericControls) updateValue(channel, "CC"+String(number), value, MIDIValueParameter::CONTROL_CHANGE, number);

	if (scriptManager->items.size() > 0) scriptManager->callFunctionOnAllItems(ccEventId, Array<var>(channel, number, value));

}

void MIDIModule::sysExReceived(const MidiMessage & msg)
{
	if (!enabled->boolValue()) return;
	inActivityTrigger->trigger(); 

	Array<uint8> data(msg.getSysExData(), msg.getSysExDataSize());

	if (logIncomingData->boolValue())
	{
		String log = "Sysex received, " + String(data.size()) + " bytes";
		for (int i = 0; i < data.size(); i++)
		{
			log += "\n" + String(data[i]);
		}
		NLOG(niceName, log);
	}

	if (scriptManager->items.size() > 0) scriptManager->callFunctionOnAllItems(sysexEventId, Array<var>(data.getRawDataPointer(), data.size()));
}

void MIDIModule::fullFrameTimecodeReceived(const MidiMessage& msg)
{
	int hours = 0, minutes = 0, seconds = 0, frames = 0;
	MidiMessage::SmpteTimecodeType timecodeType;
	msg.getFullFrameParameters(hours, minutes, seconds, frames,timecodeType);
	NLOG(niceName, "Full frame timecode received : " << hours << ":" << minutes << ":" << seconds << "." << frames << " / " << timecodeType);
}

void MIDIModule::pitchWheelReceived(const MidiMessage& msg, const int &channel, const int &value)
{
	if (!enabled->boolValue()) return;
	inActivityTrigger->trigger();
	if (logIncomingData->boolValue()) NLOG(niceName, "Pitch wheel, channel : " << channel << ", value : " << value);

	if (useGenericControls) updateValue(channel, "PitchWheel", value, MIDIValueParameter::PITCH_WHEEL, 0);

	if (scriptManager->items.size() > 0) scriptManager->callFunctionOnAllItems(pitchWheelEventId, Array<var>(channel, value));
}

var MIDIModule::sendNoteOnFromScript(const var::NativeFunctionArgs & args)
{
	MIDIModule * m = getObjectFromJS<MIDIModule>(args);
	if (!m->enabled->boolValue()) return var();

	if (args.numArguments < 3)
	{
		NLOG(m->niceName, "Not enough arguments passed from script, got " << args.numArguments << ", expected 3");
		return var();
	}

	m->sendNoteOn(args.arguments[0], args.arguments[1], args.arguments[2]);
	return var();
}

var MIDIModule::sendNoteOffFromScript(const var::NativeFunctionArgs & args)
{
	MIDIModule * m = getObjectFromJS<MIDIModule>(args);
	if (!m->enabled->boolValue()) return var();

	if (args.numArguments < 2)
	{
		NLOG(m->niceName, "Not enough arguments passed from script, got " << args.numArguments << ", expected 2");
		return var();
	}

	m->sendNoteOff(args.arguments[0], args.arguments[1]);
	return var();
}

var MIDIModule::sendCCFromScript(const var::NativeFunctionArgs & args)
{
	MIDIModule * m = getObjectFromJS<MIDIModule>(args);
	if (!m->enabled->boolValue()) return var();

	if (args.numArguments < 3)
	{
		NLOG(m->niceName, "Not enough arguments passed from script, got " << args.numArguments << ", expected 3");
		return var();
	}

	m->sendControlChange(args.arguments[0], args.arguments[1], args.arguments[2]);
	return var();
}

var MIDIModule::sendSysexFromScript(const var::NativeFunctionArgs & args)
{
	MIDIModule * m = getObjectFromJS<MIDIModule>(args);
	if (!m->enabled->boolValue()) return var();

	if (args.numArguments == 0)
	{
		NLOG(m->niceName, "Not enough arguments passed from script, got " << args.numArguments << ", expected at least 1");
		return var();
	}

	Array<uint8> data;
	for (int i = 0; i < args.numArguments; i++) data.add((uint8)(int)args.arguments[i]);
	m->sendSysex(data);

	return var();
}

var MIDIModule::sendProgramChangeFromScript(const var::NativeFunctionArgs& args)
{
	MIDIModule* m = getObjectFromJS<MIDIModule>(args);
	if (!m->enabled->boolValue()) return var();

	if (args.numArguments < 2)
	{
		NLOG(m->niceName, "Not enough arguments passed from script, got " << args.numArguments << ", expected 2");
		return var();
	}

	m->sendProgramChange(args.arguments[0], args.arguments[1]);
	return var();
}

var MIDIModule::sendPitchWheelFromScript(const var::NativeFunctionArgs& args)
{
	MIDIModule* m = getObjectFromJS<MIDIModule>(args);
	if (!m->enabled->boolValue()) return var();

	if (args.numArguments < 2)
	{
		NLOG(m->niceName, "Not enough arguments passed from script, got " << args.numArguments << ", expected 2");
		return var();
	}

	m->sendPitchWheel(args.arguments[0], args.arguments[1]);
	return var();
}

void MIDIModule::updateValue(const int & channel, const String & n, const int & val, const MIDIValueParameter::Type & type, const int & pitchOrNumber)
{
	const String nWithChannel = "[" + String(channel) + "] " + n;
	Parameter * p = dynamic_cast<Parameter *>(valuesCC.getControllableByName(nWithChannel,true));
	if (p == nullptr)
	{
		if (autoAdd->boolValue())
		{
			p = new MIDIValueParameter(nWithChannel, "Channel "+String(channel)+" : "+n, val, channel, pitchOrNumber, type);
			p->isRemovableByUser = true;
			p->saveValueOnly = false;
			valuesCC.addParameter(p);
			valuesCC.orderControllablesAlphabetically();
		}
	}
	else
	{
		p->setValue(val);
	}

}

void MIDIModule::showMenuAndCreateValue(ControllableContainer * container)
{
	MIDIModule * module = dynamic_cast<MIDIModule *>(container->parentContainer.get());
	if (module == nullptr) return;

	PopupMenu m;
	m.addItem(1, "Add Note");
	m.addItem(2, "Add Control Change");

	int mResult = m.show();
	if (mResult == 0) return;
	
	String mType = mResult == 1 ? "Note" : "Control Change";
	String mName = mResult == 1 ? "Pitch" : "Number";

	AlertWindow window("Add a "+mType, "Configure the parameters for this "+mType, AlertWindow::AlertIconType::NoIcon);
	window.addTextEditor("channel", "1", "Channel (1-16)");
	window.addTextEditor("pitch", "1", mName + "(1-127)");

	window.addButton("OK", 1, KeyPress(KeyPress::returnKey));
	window.addButton("Cancel", 0, KeyPress(KeyPress::escapeKey));

	int result = window.runModalLoop();

	if (result)
	{
		int channel = jlimit<int>(1, 16, window.getTextEditorContents("channel").getIntValue());
		int pitch = jlimit<int>(1, 127, window.getTextEditorContents("pitch").getIntValue());

		if (mResult == 1) module->noteOnReceived(channel, pitch, 0);
		else module->controlChangeReceived(channel, pitch, 0);
	}
}

void MIDIModule::loadJSONDataInternal(var data)
{
	Module::loadJSONDataInternal(data);
	valuesCC.orderControllablesAlphabetically();
	setupIOConfiguration(inputDevice != nullptr || valuesCC.controllables.size() > 0, outputDevice != nullptr);
}


MIDIModule::MIDIRouteParams::MIDIRouteParams(Module * sourceModule, Controllable * c) :
	type(nullptr),
	channel(nullptr),
	pitchOrNumber(nullptr)
{
	if (c->type != Controllable::FLOAT && c->type != Controllable::INT && c->type != Controllable::BOOL)
	{
		NLOGWARNING(niceName, c->niceName + " is of type " + c->getTypeString() + ",\nOnly float and integers can be routed through MIDI");
		return;
	}

	type = addEnumParameter("Type", "The type of MIDI Command to route");
	type->addOption("Control Change", MIDIManager::CONTROL_CHANGE)->addOption("Note On", MIDIManager::NOTE_ON)->addOption("Note Off", MIDIManager::NOTE_OFF);
	type->setValueWithData(MIDIManager::getInstance()->midiRouterDefaultType->getValueData());

	channel = addIntParameter("Channel", "The Channel", 1, 1, 16);
	pitchOrNumber = addIntParameter("Pitch / Number", "Pitch if type is a note, number if it is a controlChange", 0, 0, 127);
}


void MIDIModule::handleRoutedModuleValue(Controllable * c, RouteParams * p)
{
	MIDIRouteParams * mp = dynamic_cast<MIDIRouteParams *>(p);
	if (mp->type == nullptr) return;

	MIDIManager::MIDIEventType t = mp->type->getValueDataAsEnum<MIDIManager::MIDIEventType>();

	int value = 127;
	Parameter * sp = c->type == Controllable::TRIGGER ? nullptr : dynamic_cast<Parameter *>(c);
	if (sp != nullptr) value = sp->getNormalizedValue() * 127;

	switch (t)
	{
	case MIDIManager::NOTE_ON:
		sendNoteOn(mp->channel->intValue(), mp->pitchOrNumber->intValue(), value);
		break;

	case MIDIManager::NOTE_OFF:
		sendNoteOff(mp->channel->intValue(), mp->pitchOrNumber->intValue());
		break;

	case MIDIManager::CONTROL_CHANGE:
		sendControlChange(mp->channel->intValue(), mp->pitchOrNumber->intValue(), value);
		break;

	default:
		break;
	}
}
