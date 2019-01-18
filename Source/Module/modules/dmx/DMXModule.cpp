/*
  ==============================================================================

    DMXModule.cpp
    Created: 6 Apr 2017 10:22:10pm
    Author:  Ben

  ==============================================================================
*/

#include "DMXModule.h"
#include "commands/DMXCommand.h"

DMXModule::DMXModule() :
	Module("DMX"),
	dmxDevice(nullptr)
{
	setupIOConfiguration(false, true);
	valuesCC.editorIsCollapsed = true;
	canHandleRouteValues = true;
	includeValuesInSave = true;

	defManager.add(CommandDefinition::createDef(this, "", "Set value", &DMXCommand::create)->addParam("action", DMXCommand::SET_VALUE));
	defManager.add(CommandDefinition::createDef(this, "", "Set value 16bit", &DMXCommand::create)->addParam("action", DMXCommand::SET_VALUE_16BIT));
	defManager.add(CommandDefinition::createDef(this, "", "Set range", &DMXCommand::create)->addParam("action", DMXCommand::SET_RANGE));
	defManager.add(CommandDefinition::createDef(this, "", "Set Color", &DMXCommand::create)->addParam("action",DMXCommand::COLOR));
	defManager.add(CommandDefinition::createDef(this, "", "Clear all", &DMXCommand::create)->addParam("action", DMXCommand::CLEAR_ALL));

	dmxType = moduleParams.addEnumParameter("DMX Type", "Choose the type of dmx interface you want to connect");

	dmxType->addOption("Open DMX", DMXDevice::OPENDMX)->addOption("Enttec DMX Pro", DMXDevice::ENTTEC_DMXPRO)->addOption("Enttec DMX MkII", DMXDevice::ENTTEC_MK2)->addOption("Art-Net", DMXDevice::ARTNET);
	dmxType->setValueWithKey("Open DMX");

	autoAdd = moduleParams.addBoolParameter("Auto Add", "If checked, this will automatically add values for changed channels", true);
	autoAdd->hideInEditor = !hasInput;

	dmxConnected = moduleParams.addBoolParameter("Connected", "DMX is connected ?", false);
	dmxConnected->isControllableFeedbackOnly = true;

	valuesCC.userCanAddControllables = true;
	valuesCC.customUserCreateControllableFunc = &DMXModule::showMenuAndCreateValue;

	setCurrentDMXDevice(DMXDevice::create((DMXDevice::Type)(int)dmxType->getValueData()));
}

DMXModule::~DMXModule()
{
}

void DMXModule::setCurrentDMXDevice(DMXDevice * d)
{
	if (dmxDevice == d) return;

	if (dmxDevice != nullptr)
	{
		dmxDevice->removeDMXDeviceListener(this);
		dmxDevice->clearDevice();
		moduleParams.removeChildControllableContainer(dmxDevice);
	}

	dmxDevice = d;
	
	dmxConnected->hideInEditor = dmxDevice == nullptr || dmxDevice->type == DMXDevice::ARTNET;
	dmxConnected->setValue(false);

	if (dmxDevice != nullptr)
	{
		dmxDevice->addDMXDeviceListener(this);
		moduleParams.addChildControllableContainer(dmxDevice);
	}

	setupIOConfiguration(dmxDevice != nullptr && dmxDevice->canReceive, true);
	autoAdd->hideInEditor = !hasInput;

	dmxModuleListeners.call(&DMXModuleListener::dmxDeviceChanged);
}

void DMXModule::sendDMXValue(int channel, int value)
{
	if (dmxDevice == nullptr) return;
	if (logOutgoingData->boolValue()) NLOG(niceName, "Send DMX : " + String(channel) + " > " + String(value));
	outActivityTrigger->trigger();
	dmxDevice->sendDMXValue(channel, value);
}

void DMXModule::sendDMXValues(int startChannel, Array<int> values)
{
	if (dmxDevice == nullptr) return;
	if (logOutgoingData->boolValue())
	{
		String s = "Send DMX : " + String(startChannel) + ", " + String(values.size()) + " values";
		int ch = startChannel;
		for (auto &v : values)
		{
			s += "\nChannel " + String(ch)+" : " + String(v);
			ch++;
		}
		NLOG(niceName, s);
	}

	outActivityTrigger->trigger();

	dmxDevice->sendDMXRange(startChannel, values);
}

void DMXModule::send16BitDMXValue(int startChannel, int value, DMXByteOrder byteOrder)
{
	if (dmxDevice == nullptr) return;
	if (logOutgoingData->boolValue()) NLOG(niceName, "Send 16-bit DMX : " + String(startChannel) + " > " + String(value));
	outActivityTrigger->trigger(); 
	dmxDevice->sendDMXValue(startChannel, byteOrder == MSB ? (value >> 8) & 0xFF : value & 0xFF);
	dmxDevice->sendDMXValue(startChannel+1, byteOrder == MSB ? 0xFF : (value >> 8) & 0xFF);
}

void DMXModule::send16BitDMXValues(int startChannel, Array<int> values, DMXByteOrder byteOrder)
{
	if (dmxDevice == nullptr) return;
	if (logOutgoingData->boolValue()) NLOG(niceName, "Send 16-bit DMX : " + String(startChannel) + " > " + String(values.size()) + " values");
	outActivityTrigger->trigger();

	Array<int> dmxValues;
	int numValues = values.size();
	dmxValues.resize(numValues * 2);
	for (int i = 0; i < numValues; i++)
	{
		int value = values[i];
		dmxValues.set(i * 2, byteOrder == MSB ? (value >> 8) & 0xFF : value & 0xFF);
		dmxValues.set(i * 2 + 1, byteOrder == MSB ? 0xFF : (value >> 8) & 0xFF);
	}

	dmxDevice->sendDMXRange(startChannel, dmxValues);
}

void DMXModule::clearItem()
{
	setCurrentDMXDevice(nullptr);
}

var DMXModule::getJSONData()
{
	var data = Module::getJSONData();
	if (dmxDevice != nullptr) data.getDynamicObject()->setProperty("device", dmxDevice->getJSONData());
	return data;
}

void DMXModule::loadJSONDataInternal(var data)
{
	Module::loadJSONDataInternal(data);
	if (dmxDevice != nullptr && data.getDynamicObject()->hasProperty("device")) dmxDevice->loadJSONData(data.getProperty("device", ""));
}

void DMXModule::controllableFeedbackUpdate(ControllableContainer * cc, Controllable * c)
{
	Module::controllableFeedbackUpdate(cc, c);
	if (c == dmxType) setCurrentDMXDevice(DMXDevice::create((DMXDevice::Type)(int)dmxType->getValueData()));
}

void DMXModule::dmxDeviceConnected()
{
	dmxConnected->setValue(true);
}

void DMXModule::dmxDeviceDisconnected()
{
	dmxConnected->setValue(false);
}

void DMXModule::dmxDataInChanged(int channel, int value)
{
	if (logIncomingData->boolValue()) NLOG(niceName, "DMX In : " + String(channel) + " > " + String(value));
	inActivityTrigger->trigger();

	IntParameter * dVal = channelMap.contains(channel) ? channelMap[channel] : nullptr;
	if (dVal == nullptr)
	{
		if (!autoAdd->boolValue()) return;

		dVal = valuesCC.addIntParameter("Channel " + String(channel), "DMX Value for channel " + String(channel), 0, 0, 255);
		//dVal->setControllableFeedbackOnly(true);
		dVal->saveValueOnly = false;
		channelMap.set(channel, dVal);
	}

	dVal->setValue(value);
}

void DMXModule::showMenuAndCreateValue(ControllableContainer * container)
{
	DMXModule * module = dynamic_cast<DMXModule *>(container->parentContainer.get());
	if (module == nullptr) return;

	AlertWindow window("Add a value", "Configure the parameters for value", AlertWindow::AlertIconType::NoIcon);
	window.addTextEditor("channel", "1", "Channel (1-512)");

	window.addButton("OK", 1, KeyPress(KeyPress::returnKey));
	window.addButton("Cancel", 0, KeyPress(KeyPress::escapeKey));

	int result = window.runModalLoop();

	if (result)
	{
		int channel = jlimit<int>(1, 512, window.getTextEditorContents("channel").getIntValue());
		module->dmxDataInChanged(channel, 0);
	}
}

DMXModule::DMXRouteParams::DMXRouteParams(Module * sourceModule, Controllable * c) :
	mode16bit(nullptr),
	channel(nullptr),
	value(nullptr)
{
	if (c->type == Controllable::FLOAT || c->type == Controllable::INT || c->type == Controllable::BOOL || c->type == Controllable::POINT2D || c->type == Controllable::POINT3D)
	{
		mode16bit = addEnumParameter("Mode", "Choosing the resolution and Byte order for this routing");
		mode16bit->addOption("8-bit", BIT8)->addOption("16-bit MSB", MSB)->addOption("16-bit LSB", LSB);
	}

	if (c->type == Controllable::FLOAT || c->type == Controllable::BOOL || c->type == Controllable::POINT2D || c->type == Controllable::POINT3D)
	{
		fullRange = addBoolParameter("Full Range", "If checked, value will be remapped from 0-1 will to 0-255 (or 0-65535 in 16-bit mode)", true);
	}

	channel = addIntParameter("Channel", "The Channel", 1, 1, 16);
}

void DMXModule::handleRoutedModuleValue(Controllable * c, RouteParams * p)
{
	DMXRouteParams * rp = dynamic_cast<DMXRouteParams *>(p);
	
	Parameter * sp = c->type == Controllable::TRIGGER ? nullptr : dynamic_cast<Parameter *>(c);

	bool fullRange = rp->fullRange != nullptr ? rp->fullRange->boolValue() : false;
	
	DMXByteOrder byteOrder = rp->mode16bit->getValueDataAsEnum<DMXByteOrder>();

	switch (c->type)
	{
	case Controllable::BOOL:
	case Controllable::INT:
	case Controllable::FLOAT:
		if (byteOrder == BIT8) sendDMXValue(rp->channel->intValue(), fullRange?sp->getNormalizedValue()*255:(float)sp->getValue());
		else send16BitDMXValue(rp->channel->intValue(), fullRange?sp->getNormalizedValue()*65535:(float)sp->getValue(), byteOrder);
		break;

	case Controllable::POINT2D:
	{
		Point<float> pp = ((Point2DParameter *)sp)->getPoint();
		if (fullRange) pp *= byteOrder != BIT8 ? 65535 : 255;

		Array<int> values;
		values.add((int)pp.x, (int)pp.y);

		if (byteOrder == BIT8) sendDMXValues(rp->channel->intValue(), values);
		else send16BitDMXValues(rp->channel->intValue(), values, byteOrder);
	}
	break;

	case Controllable::POINT3D:
	{
		Vector3D<float> pp = ((Point3DParameter *)sp)->getVector();
		if (fullRange) pp *= byteOrder != BIT8 ? 65535 : 255;

		Array<int> values;
		values.add((int)pp.x, (int)pp.y, (int)pp.z);

		if (byteOrder == BIT8) sendDMXValues(rp->channel->intValue(), values);
		else send16BitDMXValues(rp->channel->intValue(), values, byteOrder);
	}
	break;

	case Controllable::COLOR:
	{
		Colour col = ((ColorParameter *)sp)->getColor();
		Array<int> values;
		values.add(col.getRed(), col.getGreen(), col.getBlue());
		sendDMXValues(rp->channel->intValue(), values);
	}

	break;
            
        default:
            break;
	}
}
