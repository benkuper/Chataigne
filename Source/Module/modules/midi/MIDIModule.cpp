/*
  ==============================================================================

    MIDIModule.cpp
    Created: 20 Dec 2016 12:35:26pm
    Author:  Ben

  ==============================================================================
*/

#include "MIDIModule.h"
#include "commands/MIDICommands.h"

MIDIModule::MIDIModule(const String & name, bool _useGenericControls) :
	Module(name),
	inputDevice(nullptr),
	outputDevice(nullptr),
	useGenericControls(_useGenericControls)
{
	setupIOConfiguration(true, true);

	canHandleRouteValues = true;

	if (useGenericControls)
	{
		autoAdd = moduleParams.addBoolParameter("Auto Add", "Auto Add MIDI values that are received but not in the list", true);
		defManager.add(CommandDefinition::createDef(this, "", "Note On", &MIDINoteAndCCCommand::create)->addParam("type", (int)MIDINoteAndCCCommand::NOTE_ON));
		defManager.add(CommandDefinition::createDef(this, "", "Note Off", &MIDINoteAndCCCommand::create)->addParam("type", (int)MIDINoteAndCCCommand::NOTE_OFF));
		defManager.add(CommandDefinition::createDef(this, "", "Full Note", &MIDINoteAndCCCommand::create)->addParam("type", (int)MIDINoteAndCCCommand::FULL_NOTE));
		defManager.add(CommandDefinition::createDef(this, "", "Controller Change", &MIDINoteAndCCCommand::create)->addParam("type", (int)MIDINoteAndCCCommand::CONTROLCHANGE));
		defManager.add(CommandDefinition::createDef(this, "", "Sysex Message", &MIDISysExCommand::create));
	}
	
	autoFeedback = moduleParams.addBoolParameter("Auto Feedback", "If checked, all changed values will be resent automatically to the outputs", false);
	autoFeedback->isTargettable = false;

	midiParam = new MIDIDeviceParameter("Devices");
	moduleParams.addParameter(midiParam);

	setupIOConfiguration(inputDevice != nullptr, outputDevice != nullptr);
}

MIDIModule::~MIDIModule()
{
	if(inputDevice != nullptr) inputDevice->removeMIDIInputListener(this);
	if (outputDevice != nullptr) outputDevice->close();
}


void MIDIModule::sendNoteOn(int pitch, int velocity, int channel)
{
	if (!enabled->boolValue()) return;
	if (outputDevice == nullptr) return;
	if (logOutgoingData->boolValue()) NLOG(niceName, "Send Note on " << MIDIManager::getNoteName(pitch) << ", " << velocity << ", " << channel);
	outActivityTrigger->trigger();
	outputDevice->sendNoteOn(pitch, velocity, channel);
}

void MIDIModule::sendNoteOff(int pitch, int channel)
{
	if (!enabled->boolValue()) return;
	if (outputDevice == nullptr) return;
	if (logOutgoingData->boolValue()) NLOG(niceName, "Send Note off " << MIDIManager::getNoteName(pitch) << ", " << channel);
	outActivityTrigger->trigger();
	outputDevice->sendNoteOff(pitch, channel);
}

void MIDIModule::sendControlChange(int number, int value, int channel)
{
	if (!enabled->boolValue()) return;
	if (outputDevice == nullptr) return;
	if (logOutgoingData->boolValue()) NLOG(niceName, "Send Control Change " << number << ", " << value << ", " << channel);
	outActivityTrigger->trigger();
	outputDevice->sendControlChange(number,value,channel);
}

void MIDIModule::sendSysex(Array<uint8> data)
{
	if (!enabled->boolValue()) return;
	if (outputDevice == nullptr) return;
	if (logOutgoingData->boolValue()) NLOG(niceName, "Send Sysex " << data.size() << " bytes");
	outActivityTrigger->trigger();
	outputDevice->sendSysEx(data);
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
				case MIDIValueParameter::NOTE_ON: sendNoteOn(mvp->pitchOrNumber, mvp->intValue(), mvp->channel); break;
				case MIDIValueParameter::NOTE_OFF: sendNoteOff(mvp->pitchOrNumber, mvp->channel); break;
				case MIDIValueParameter::CONTROL_CHANGE: sendControlChange(mvp->pitchOrNumber, mvp->intValue(), mvp->channel); break;
				}
			}
		}
	}
}

void MIDIModule::updateMIDIDevices()
{
	MIDIInputDevice * newInput = midiParam->inputDevice;
	if (inputDevice != newInput)
	{
		if (inputDevice != nullptr)
		{
			inputDevice->removeMIDIInputListener(this);
		}
		inputDevice = newInput;
		if (inputDevice != nullptr)
		{
			inputDevice->addMIDIInputListener(this);
		}
	}

	MIDIOutputDevice * newOutput = midiParam->outputDevice;
	if (outputDevice != newOutput)
	{
		if(outputDevice != nullptr) outputDevice->close();
		outputDevice = newOutput;
		if(outputDevice != nullptr) outputDevice->open();
	} 

	setupIOConfiguration(inputDevice != nullptr || valuesCC.controllables.size() > 0, outputDevice != nullptr);
}

void MIDIModule::noteOnReceived(const int & channel, const int & pitch, const int & velocity)
{
	if (!enabled->boolValue()) return; 
	inActivityTrigger->trigger();
	if (logIncomingData->boolValue())  NLOG(niceName, "Note On : " << channel << ", " << MIDIManager::getNoteName(pitch) << ", " << velocity);

	if (useGenericControls) updateValue(channel, MIDIManager::getNoteName(pitch), velocity, MIDIValueParameter::NOTE_ON, pitch);
}

void MIDIModule::noteOffReceived(const int & channel, const int & pitch, const int & velocity)
{
	if (!enabled->boolValue()) return; 
	inActivityTrigger->trigger();
	if (logIncomingData->boolValue()) NLOG(niceName, "Note Off : " << channel << ", " << MIDIManager::getNoteName(pitch) << ", " << velocity);

	if (useGenericControls) updateValue(channel, MIDIManager::getNoteName(pitch), velocity, MIDIValueParameter::NOTE_OFF, pitch);
	
}

void MIDIModule::controlChangeReceived(const int & channel, const int & number, const int & value)
{
	if (!enabled->boolValue()) return; 
	inActivityTrigger->trigger();
	if (logIncomingData->boolValue()) NLOG(niceName, "Control Change : " << channel << ", " << number << ", " << value);

	if (useGenericControls) updateValue(channel, "CC"+String(number), value, MIDIValueParameter::CONTROL_CHANGE, number);

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

void MIDIModule::handleRoutedModuleValue(Controllable * c, RouteParams * p)
{
	MIDIRouteParams * mp = dynamic_cast<MIDIRouteParams *>(p);
	if (mp->type == nullptr) return;

	MIDIRouteParams::Type t = mp->type->getValueDataAsEnum<MIDIRouteParams::Type>();

	int value = 127;
	Parameter * sp = c->type == Controllable::TRIGGER?nullptr:dynamic_cast<Parameter *>(c);
	if (sp != nullptr) value = sp->getNormalizedValue()*127;

	switch (t)
	{
	case MIDIRouteParams::NOTE_ON:
		sendNoteOn(mp->pitchOrNumber->intValue(), value, mp->channel->intValue());
		break;

	case MIDIRouteParams::NOTE_OFF:
		sendNoteOff(mp->pitchOrNumber->intValue(), mp->channel->intValue());
		break;

	case MIDIRouteParams::CONTROL_CHANGE:
		sendControlChange(mp->pitchOrNumber->intValue(), value, mp->channel->intValue());
		break;
            
        default:
            break;
	}
}

var MIDIModule::getJSONData()
{
	var data = Module::getJSONData();
	data.getDynamicObject()->setProperty("values", valuesCC.getJSONData());
	return data;
}

void MIDIModule::loadJSONDataInternal(var data)
{
	Module::loadJSONDataInternal(data);
	valuesCC.loadJSONData(data.getProperty("values", var()), true);
	valuesCC.orderControllablesAlphabetically();

	setupIOConfiguration(inputDevice != nullptr || valuesCC.controllables.size() > 0, outputDevice != nullptr);
}

/*
InspectableEditor * MIDIModule::getEditor(bool isRoot)
{
	return new MIDIModuleEditor(this,isRoot);
}
*/

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
	type->addOption("Control Change", CONTROL_CHANGE)->addOption("Note On", NOTE_ON)->addOption("Note Off",NOTE_OFF);

	channel = addIntParameter("Channel", "The Channel", 1, 1, 16);
	pitchOrNumber = addIntParameter("Pitch / Number", "Pitch if type is a note, number if it is a controlChange", 0, 0, 127);
}
