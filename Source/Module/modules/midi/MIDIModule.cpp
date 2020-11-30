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
#include "Module/ModuleManager.h"
#include "Module/Routing/ModuleRouter.h"

MIDIValueComparator MIDIModule::midiValueComparator;

MIDIModule::MIDIModule(const String & name, bool _useGenericControls) :
	Module(name),
	manualAddMode(false),
    inputDevice(nullptr),
	outputDevice(nullptr),
    useGenericControls(_useGenericControls)
{
	valuesCC.customControllableComparator = &MIDIModule::midiValueComparator;

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
		defManager->add(CommandDefinition::createDef(this, "", "Program Change", &MIDINoteAndCCCommand::create)->addParam("type", (int)MIDINoteAndCCCommand::PROGRAMCHANGE));
		defManager->add(CommandDefinition::createDef(this, "", "Pitch Wheel", &MIDINoteAndCCCommand::create)->addParam("type", (int)MIDINoteAndCCCommand::PITCH_WHEEL));
		defManager->add(CommandDefinition::createDef(this, "", "Channel Pressure", &MIDINoteAndCCCommand::create)->addParam("type", (int)MIDINoteAndCCCommand::CHANNEL_PRESSURE));
		defManager->add(CommandDefinition::createDef(this, "", "After Touch", &MIDINoteAndCCCommand::create)->addParam("type", (int)MIDINoteAndCCCommand::AFTER_TOUCH));
	}
	
	useHierarchy = moduleParams.addBoolParameter("Use Hierarchy", "If checked, incoming messages will be sorted in nested containers instead of 1-level", false);
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
	scriptObject.setMethod(sendChannelPressureId, &MIDIModule::sendChannelPressureFromScript);
	scriptObject.setMethod(sendAfterTouchId, &MIDIModule::sendAfterTouchFromScript);

	scriptManager->scriptTemplate += ChataigneAssetManager::getInstance()->getScriptTemplate("midi");

	valuesCC.userCanAddControllables = true;
	valuesCC.customUserCreateControllableFunc = &MIDIModule::showMenuAndCreateValue;
	alwaysShowValues = true;


	thruManager.reset(new ControllableContainer("Pass-through"));
	thruManager->userCanAddControllables = true;
	thruManager->customUserCreateControllableFunc = &MIDIModule::createThruControllable;
	moduleParams.addChildControllableContainer(thruManager.get());	
	
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
	if (logOutgoingData->boolValue()) NLOG(niceName, "Send Note on, channel : " << channel << ", pitch : " << MIDIManager::getNoteName(pitch) << ", velociy " << velocity);
	outActivityTrigger->trigger();
	outputDevice->sendNoteOn(channel, pitch, velocity);
}

void MIDIModule::sendNoteOff(int channel, int pitch)
{
	if (!enabled->boolValue()) return;
	if (outputDevice == nullptr) return;
	if (logOutgoingData->boolValue()) NLOG(niceName, "Send Note off, channel : " << channel << ", pitch");
	outActivityTrigger->trigger();
	outputDevice->sendNoteOff(channel, pitch);
}

void MIDIModule::sendControlChange(int channel, int number, int value)
{
	if (!enabled->boolValue()) return;
	if (outputDevice == nullptr) return;
	if (logOutgoingData->boolValue()) NLOG(niceName, "Send Control Change, channel : " << channel << ", number : " << number << ", value " << value);
	outActivityTrigger->trigger();
	outputDevice->sendControlChange(channel, number, value);
}

void MIDIModule::sendProgramChange(int channel, int program)
{
	if (!enabled->boolValue()) return;
	if (outputDevice == nullptr) return;
	if (logOutgoingData->boolValue()) NLOG(niceName, "Send ProgramChange, channel : " << channel << ", program : " << program);
	outActivityTrigger->trigger();
	outputDevice->sendProgramChange(channel, program);
}

void MIDIModule::sendSysex(Array<uint8> data)
{
	if (!enabled->boolValue()) return;
	if (outputDevice == nullptr) return;
	if (logOutgoingData->boolValue())
	{
		String s = "Send Sysex " + String(data.size()) + " bytes : ";
		for (int i = 0; i < data.size(); ++i) s += "\n" + String(data[i]);
		NLOG(niceName, s);
	}
	outActivityTrigger->trigger();
	outputDevice->sendSysEx(data);
}

void MIDIModule::sendPitchWheel(int channel, int value)
{
	if (!enabled->boolValue()) return;
	if (outputDevice == nullptr) return;
	if (logOutgoingData->boolValue()) NLOG(niceName, "Send PitchWheel channel : " << channel << ", value : " << value);
	outputDevice->sendPitchWheel(channel, value);
}

void MIDIModule::sendChannelPressure(int channel, int value)
{
	if (!enabled->boolValue()) return;
	if (outputDevice == nullptr) return;
	if (logOutgoingData->boolValue()) NLOG(niceName, "Send Channel Pressure channel : " << channel << ", value : " << value);
	outputDevice->sendChannelPressure(channel, value);
}

void MIDIModule::sendAfterTouch(int channel, int note, int value)
{
	if (!enabled->boolValue()) return;
	if (outputDevice == nullptr) return;
	if (logOutgoingData->boolValue()) NLOG(niceName, "Send After touch channel : " << channel << ", note : " << note << ", value : " << value);
	outputDevice->sendAfterTouch(channel, note, value);
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
	if (!enabled->boolValue() && !manualAddMode) return; 
	inActivityTrigger->trigger();
	
	if (logIncomingData->boolValue())  NLOG(niceName, "Note On : " << channel << ", " << MIDIManager::getNoteName(pitch) << ", " << velocity);

	if (useGenericControls) updateValue(channel, MIDIManager::getNoteName(pitch), velocity, MIDIValueParameter::NOTE_ON, pitch);

	if (scriptManager->items.size() > 0) scriptManager->callFunctionOnAllItems(noteOnEventId, Array<var>(channel, pitch, velocity));
}

void MIDIModule::noteOffReceived(const int & channel, const int & pitch, const int & velocity)
{
	if (!enabled->boolValue() && !manualAddMode) return;
	inActivityTrigger->trigger();
	if (logIncomingData->boolValue()) NLOG(niceName, "Note Off : " << channel << ", " << MIDIManager::getNoteName(pitch) << ", " << velocity);

	if (useGenericControls) updateValue(channel, MIDIManager::getNoteName(pitch), velocity, MIDIValueParameter::NOTE_OFF, pitch);

	if (scriptManager->items.size() > 0) scriptManager->callFunctionOnAllItems(noteOffEventId, Array<var>(channel, pitch, velocity));
	
}

void MIDIModule::controlChangeReceived(const int & channel, const int & number, const int & value)
{
	if (!enabled->boolValue() && !manualAddMode) return;
	inActivityTrigger->trigger();
	if (logIncomingData->boolValue()) NLOG(niceName, "Control Change : " << channel << ", " << number << ", " << value);

	if (useGenericControls) updateValue(channel, "CC"+String(number), value, MIDIValueParameter::CONTROL_CHANGE, number);

	if (scriptManager->items.size() > 0) scriptManager->callFunctionOnAllItems(ccEventId, Array<var>(channel, number, value));

}

void MIDIModule::sysExReceived(const MidiMessage & msg)
{
	if (!enabled->boolValue() && !manualAddMode) return;
	inActivityTrigger->trigger();

	Array<uint8> data(msg.getSysExData(), msg.getSysExDataSize());

	if (logIncomingData->boolValue())
	{
		String log = "Sysex received, " + String(data.size()) + " bytes";
		for (int i = 0; i < data.size(); ++i)
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

void MIDIModule::pitchWheelReceived(const int &channel, const int &value)
{
	if (!enabled->boolValue() && !manualAddMode) return;
	inActivityTrigger->trigger();
	if (logIncomingData->boolValue()) NLOG(niceName, "Pitch wheel, channel : " << channel << ", value : " << value);

	if (useGenericControls) updateValue(channel, "PitchWheel", value, MIDIValueParameter::PITCH_WHEEL, 0);

	if (scriptManager->items.size() > 0) scriptManager->callFunctionOnAllItems(pitchWheelEventId, Array<var>(channel, value));
}

void MIDIModule::channelPressureReceived(const int& channel, const int& value)
{
	if (!enabled->boolValue() && !manualAddMode) return;
	inActivityTrigger->trigger();
	if (logIncomingData->boolValue()) NLOG(niceName, "Channel Pressure, channel : " << channel << ", value : " << value);

	if (useGenericControls) updateValue(channel, "ChannelPressure", value, MIDIValueParameter::CHANNEL_PRESSURE, 0);

	if (scriptManager->items.size() > 0) scriptManager->callFunctionOnAllItems(channelPressureId, Array<var>(channel, value));
}

void MIDIModule::afterTouchReceived(const int& channel, const int& note, const int& value)
{
	if (!enabled->boolValue() && !manualAddMode) return;
	inActivityTrigger->trigger();
	if (logIncomingData->boolValue()) NLOG(niceName, "After Touch, channel : " << channel << ", note : " << note <<", value : " << value);

	if (useGenericControls) updateValue(channel, "AfterTouch "+ MIDIManager::getNoteName(note), value, MIDIValueParameter::AFTER_TOUCH, note);

	if (scriptManager->items.size() > 0) scriptManager->callFunctionOnAllItems(afterTouchId, Array<var>(channel, note, value));
}

void MIDIModule::midiMessageReceived(const MidiMessage& msg)
{
	if (thruManager != nullptr)
	{
		for (auto& c : thruManager->controllables)
		{
			if (TargetParameter* mt = (TargetParameter*)c)
			{
				if (!mt->enabled) continue;
				if (MIDIModule * m = (MIDIModule*)(mt->targetContainer.get()))
				{
					if (m->midiParam->outputDevice != nullptr && m->midiParam->outputDevice->device != nullptr)
					{
						m->midiParam->outputDevice->device->sendMessageNow(msg);
					}
				}
			}
		}
	}
}

var MIDIModule::sendNoteOnFromScript(const var::NativeFunctionArgs & args)
{
	MIDIModule * m = getObjectFromJS<MIDIModule>(args);
	if (!m->enabled->boolValue()) return var();
	if (!checkNumArgs(m->niceName, args, 3)) return var();


	m->sendNoteOn(args.arguments[0], args.arguments[1], args.arguments[2]);
	return var();
}

var MIDIModule::sendNoteOffFromScript(const var::NativeFunctionArgs & args)
{
	MIDIModule * m = getObjectFromJS<MIDIModule>(args);
	if (!m->enabled->boolValue()) return var();
	if (!checkNumArgs(m->niceName, args, 2)) return var();


	m->sendNoteOff(args.arguments[0], args.arguments[1]);
	return var();
}

var MIDIModule::sendCCFromScript(const var::NativeFunctionArgs & args)
{
	MIDIModule * m = getObjectFromJS<MIDIModule>(args);
	if (!m->enabled->boolValue()) return var();
	if (!checkNumArgs(m->niceName, args, 3)) return var();

	m->sendControlChange(args.arguments[0], args.arguments[1], args.arguments[2]);
	return var();
}

var MIDIModule::sendSysexFromScript(const var::NativeFunctionArgs & args)
{
	MIDIModule * m = getObjectFromJS<MIDIModule>(args);
	if (!m->enabled->boolValue()) return var();
	if (!checkNumArgs(m->niceName, args, 1)) return var();

	Array<const var *> allArgs;
	for (int i = 0; i < args.numArguments; ++i)
	{
		if (args.arguments[i].isArray())
		{
			for (int j = 0; j < args.arguments[i].size(); j++)
			{
				allArgs.add(&args.arguments[i][j]);
			}
		}
		else
		{
			allArgs.add(&args.arguments[i]);
		}
	}

	int numArgs = allArgs.size();

	Array<uint8> data;
	for (int i = 0; i < numArgs; ++i)
	{
		const var * a = allArgs[i];
		if (a->isInt() || a->isInt64() || a->isDouble() || a->isBool())data.add((uint8)(int)*a);
		else if (a->isString())
		{
			String s = a->toString();
			for (int j = 0; j < s.length(); j++)
			{
				data.add((uint8)s[j]);
			}
		}
	}
	m->sendSysex(data);

	return var();
}

var MIDIModule::sendProgramChangeFromScript(const var::NativeFunctionArgs& args)
{
	MIDIModule* m = getObjectFromJS<MIDIModule>(args);
	if (!m->enabled->boolValue()) return var();
	if (!checkNumArgs(m->niceName, args, 2)) return var();


	m->sendProgramChange(args.arguments[0], args.arguments[1]);
	return var();
}

var MIDIModule::sendPitchWheelFromScript(const var::NativeFunctionArgs& args)
{
	MIDIModule* m = getObjectFromJS<MIDIModule>(args);
	if (!m->enabled->boolValue()) return var();
	if (!checkNumArgs(m->niceName, args, 2)) return var();


	m->sendPitchWheel(args.arguments[0], args.arguments[1]);
	return var();
}

var MIDIModule::sendChannelPressureFromScript(const var::NativeFunctionArgs& args)
{
	MIDIModule* m = getObjectFromJS<MIDIModule>(args);
	if (!m->enabled->boolValue()) return var();
	if (!checkNumArgs(m->niceName, args, 2)) return var();
	m->sendChannelPressure(args.arguments[0], args.arguments[1]);
	return var();
}

var MIDIModule::sendAfterTouchFromScript(const var::NativeFunctionArgs& args)
{
	MIDIModule* m = getObjectFromJS<MIDIModule>(args);
	if (!m->enabled->boolValue()) return var();
	if (!checkNumArgs(m->niceName, args, 3)) return var();

	m->sendAfterTouch(args.arguments[0], args.arguments[1], args.arguments[2]);
	return var();
}

void MIDIModule::updateValue(const int & channel, const String & n, const int & val, const MIDIValueParameter::Type & type, const int & pitchOrNumber)
{
	ControllableContainer* cParentContainer = &valuesCC;

	String pName = n;

	if (useHierarchy->boolValue())
	{
		ControllableContainer* channelContainer = valuesCC.getControllableContainerByName("Channel " + String(channel), true);
		if (channelContainer == nullptr)
		{
			if (!autoAdd->boolValue()) return;

			channelContainer = new ControllableContainer("Channel " + String(channel));
			channelContainer->saveAndLoadRecursiveData = true;
			channelContainer->isRemovableByUser = true;
			valuesCC.addChildControllableContainer(channelContainer, true);
		}

		String typeName;
		switch (type)
		{
		case MIDIValueParameter::NOTE_ON:
		case MIDIValueParameter::NOTE_OFF:
			typeName = "Notes";
			break;

		case MIDIValueParameter::CONTROL_CHANGE:
			typeName = "Control Change";
			break;

		default:
			typeName = "Other";
		}

		ControllableContainer* typeContainer = channelContainer->getControllableContainerByName(typeName, true);
		if (typeContainer == nullptr)
		{
			if (!autoAdd->boolValue()) return;

			typeContainer = new ControllableContainer(typeName);
			typeContainer->saveAndLoadRecursiveData = true;
			typeContainer->isRemovableByUser = true;
			typeContainer->customControllableComparator = &MIDIModule::midiValueComparator;
			channelContainer->addChildControllableContainer(typeContainer, true);
		}

		cParentContainer = typeContainer;
	}
	else
	{
		pName = "[" + String(channel) + "] " + n;
	}
	
	Parameter * p = dynamic_cast<Parameter *>(cParentContainer->getControllableByName(pName,true));

	if (p == nullptr)
	{
		if (autoAdd->boolValue() || manualAddMode)
		{
			p = new MIDIValueParameter(pName, "Channel "+String(channel)+" : "+n, 0, channel, pitchOrNumber, type);
			p->setValue(val);
			p->isRemovableByUser = true;
			p->saveValueOnly = false;
			cParentContainer->addParameter(p);
			cParentContainer->sortControllables();
		}
	}
	else if(!manualAddMode)
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
	m.addItem(3, "Add Pitch Wheel");

	int mResult = m.show();
	if (mResult == 0) return;
	
	String mType = mResult == 1 ? "Note" : mResult == 3 ? "Pitch Wheel" : "Control Change";

	AlertWindow window("Add a "+mType, "Configure the parameters for this "+mType, AlertWindow::AlertIconType::NoIcon);
	window.addTextEditor("channel", "1", "Channel (1-16)");

	if (mResult != 3)
	{
		String mName = mResult == 1 ? "Pitch" : "Number";
		window.addTextEditor("pitch", "1", mName + "(1-127)");
	}

	window.addButton("OK", 1, KeyPress(KeyPress::returnKey));
	window.addButton("Cancel", 0, KeyPress(KeyPress::escapeKey));

	int result = window.runModalLoop();

	if (result)
	{
		int channel = jlimit<int>(1, 16, window.getTextEditorContents("channel").getIntValue());

		if (mResult == 3)
		{
			module->pitchWheelReceived(channel, 0);
		}
		else
		{
			int pitch = jlimit<int>(1, 127, window.getTextEditorContents("pitch").getIntValue());
			
			module->manualAddMode = true;
			if (mResult == 1) module->noteOnReceived(channel, pitch, 0);
			else module->controlChangeReceived(channel, pitch, 0);
			module->manualAddMode = false;
		}
	}
}

void MIDIModule::createThruControllable(ControllableContainer* cc)
{
	TargetParameter* p = new TargetParameter("Output module", "Target module to send the raw data to", "");
	p->targetType = TargetParameter::CONTAINER;
	p->customGetTargetContainerFunc = &ModuleManager::showAndGetModuleOfType<MIDIModule>;
	p->isRemovableByUser = true;
	p->canBeDisabledByUser = true;
	p->saveValueOnly = false;
	cc->addParameter(p);
}

void MIDIModule::loadJSONDataInternal(var data)
{
	Module::loadJSONDataInternal(data);
	valuesCC.sortControllables();
	setupIOConfiguration(inputDevice != nullptr || valuesCC.controllables.size() > 0, outputDevice != nullptr);

	if (thruManager != nullptr)
	{
		//thruManager->loadJSONData(data.getProperty("thru", var()));
		for (auto& c : thruManager->controllables)
		{
			if (TargetParameter* mt = dynamic_cast<TargetParameter*>(c))
			{
				mt->targetType = TargetParameter::CONTAINER;
				mt->customGetTargetContainerFunc = &ModuleManager::showAndGetModuleOfType<MIDIModule>;
				mt->isRemovableByUser = true;
				mt->canBeDisabledByUser = true;
			}
		}
	}
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
	type->addOption("Control Change", MIDIManager::CONTROL_CHANGE)->addOption("Note On", MIDIManager::NOTE_ON)->addOption("Note Off", MIDIManager::NOTE_OFF)->addOption("Pitch Wheel", MIDIManager::PITCH_WHEEL);
	type->setValueWithData(MIDIManager::getInstance()->midiRouterDefaultType->getValueData());

	channel = addIntParameter("Channel", "The Channel", 1, 1, 16);

	if (type->getValueDataAsEnum<MIDIManager::MIDIEventType>() != MIDIManager::PITCH_WHEEL)
	{
		pitchOrNumber = addIntParameter("Pitch / Number", "Pitch if type is a note, number if it is a controlChange", 0, 0, 127);
	}
}

void MIDIModule::MIDIRouteParams::onContainerParameterChanged(Parameter  * p)
{
	RouteParams::onContainerParameterChanged(p);

	if (p == type)
	{
		if (type->getValueDataAsEnum<MIDIManager::MIDIEventType>() != MIDIManager::PITCH_WHEEL)
		{
			if(pitchOrNumber == nullptr) pitchOrNumber = addIntParameter("Pitch / Number", "Pitch if type is a note, number if it is a controlChange", 0, 0, 127);
		}
		else
		{
			if (pitchOrNumber != nullptr)
			{
				removeControllable(pitchOrNumber);
				pitchOrNumber = nullptr;
			}
		}
	}
}


void MIDIModule::handleRoutedModuleValue(Controllable * c, RouteParams * p)
{
	if (c == nullptr || p == nullptr) return;

	if (MIDIRouteParams* mp = dynamic_cast<MIDIRouteParams*>(p))
	{
		if (mp->type == nullptr) return;

		MIDIManager::MIDIEventType t = mp->type->getValueDataAsEnum<MIDIManager::MIDIEventType>();

		int value = 127;
		Parameter* sp = c->type == Controllable::TRIGGER ? nullptr : dynamic_cast<Parameter*>(c);
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

		case MIDIManager::PITCH_WHEEL:
			sendPitchWheel(mp->channel->intValue(), value);
			break;

		default:
			break;
		}
	}
}

MIDIModule::MIDIModuleRouterController::MIDIModuleRouterController(ModuleRouter* router) :
	ModuleRouterController(router)
{
	setAllCC = addTrigger("Set All ControlChange", "Auto set all to control change");
	setAllNote = addTrigger("Set All Notes", "Auto set all to notes");
	autoSetPitch = addTrigger("Auto-set pitch", "Auto set all notes and controlChange to incrementing pitch/number");
}

void MIDIModule::MIDIModuleRouterController::triggerTriggered(Trigger* t)
{
	if (t == setAllCC || t == setAllNote)
	{
		MIDIManager::MIDIEventType midiType = t == setAllCC ? MIDIManager::CONTROL_CHANGE : MIDIManager::NOTE_ON;

		for (auto& mrv : router->sourceValues.items)
		{
			if (MIDIRouteParams* dp = dynamic_cast<MIDIRouteParams*>(mrv->routeParams.get()))
			{
				if (dp->type != nullptr) dp->type->setValueWithData(midiType); //only MIDIRouteParams with type
			}
		}
	}
	else if (t == autoSetPitch)
	{
		int startIndex = 0;
		for (auto& mrv : router->sourceValues.items)
		{
			if (MIDIRouteParams* dp = dynamic_cast<MIDIRouteParams*>(mrv->routeParams.get()))
			{
				if (dp->pitchOrNumber != nullptr) dp->pitchOrNumber->setValue(startIndex++);
			}
		}
	}
}

int MIDIValueComparator::compareElements(Controllable* c1, Controllable* c2)
{
	MIDIValueParameter* m1 = dynamic_cast<MIDIValueParameter*>(c1);
	MIDIValueParameter* m2 = dynamic_cast<MIDIValueParameter*>(c2);
	if (m1 == nullptr || m2 == nullptr) return 0;
	
	int v1 = m1->channel * 10000 + m1->type * 1000 + m1->pitchOrNumber;
	int v2 = m2->channel * 10000 + m2->type * 1000 + m2->pitchOrNumber;

	if (v1 > v2) return 1;
	else if (v1 < v2) return -1;
	return 0;
}
