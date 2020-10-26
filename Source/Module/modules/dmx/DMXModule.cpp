/*
  ==============================================================================

    DMXModule.cpp
    Created: 6 Apr 2017 10:22:10pm
    Author:  Ben

  ==============================================================================
*/

#include "DMXModule.h"
#include "commands/DMXCommand.h"
#include "Module/Routing/ModuleRouter.h"
#include "ui/DMXModuleUI.h"

DMXModule::DMXModule() :
	Module("DMX"),
	dmxDevice(nullptr)
{
	setupIOConfiguration(false, true);
	valuesCC.editorIsCollapsed = true;
	canHandleRouteValues = true;

	defManager->add(CommandDefinition::createDef(this, "", "Black out", &DMXCommand::create)->addParam("action", DMXCommand::BLACK_OUT));
	defManager->add(CommandDefinition::createDef(this, "", "Set value", &DMXCommand::create)->addParam("action", DMXCommand::SET_VALUE));
	defManager->add(CommandDefinition::createDef(this, "", "Set value 16bit", &DMXCommand::create)->addParam("action", DMXCommand::SET_VALUE_16BIT));
	defManager->add(CommandDefinition::createDef(this, "", "Set range", &DMXCommand::create)->addParam("action", DMXCommand::SET_RANGE));
	defManager->add(CommandDefinition::createDef(this, "", "Set all", &DMXCommand::create)->addParam("action", DMXCommand::SET_ALL));
	defManager->add(CommandDefinition::createDef(this, "", "Set custom values", &DMXCommand::create)->addParam("action", DMXCommand::SET_CUSTOM));
	defManager->add(CommandDefinition::createDef(this, "", "Set Color", &DMXCommand::create)->addParam("action",DMXCommand::COLOR));
	
	dmxType = moduleParams.addEnumParameter("DMX Type", "Choose the type of dmx interface you want to connect");

	dmxType->addOption("Open DMX", DMXDevice::OPENDMX)->addOption("Enttec DMX Pro", DMXDevice::ENTTEC_DMXPRO)->addOption("Enttec DMX MkII", DMXDevice::ENTTEC_MK2)->addOption("Art-Net", DMXDevice::ARTNET);
	dmxType->setValueWithKey("Open DMX");

	dmxConnected = moduleParams.addBoolParameter("Connected", "DMX is connected ?", false);
	dmxConnected->isControllableFeedbackOnly = true;
	dmxConnected->isSavable = false;
	connectionFeedbackRef = dmxConnected;

	//valuesCC.userCanAddControllables = true;
	//valuesCC.customUserCreateControllableFunc = &DMXModule::showMenuAndCreateValue;

	setCurrentDMXDevice(DMXDevice::create((DMXDevice::Type)(int)dmxType->getValueData()));

	//Script
	scriptObject.setMethod(sendDMXId, DMXModule::sendDMXFromScript);

	for (int i = 0; i < 512; ++i)
	{
		int channel = i + 1;
		DMXValueParameter* dVal = new DMXValueParameter("Channel " + String(channel), "DMX Value for channel " + String(channel), channel, 0, DMXByteOrder::BIT8);

		valuesCC.addParameter(dVal);
		channelValues.add(dVal);
	}
}

DMXModule::~DMXModule()
{
}

void DMXModule::setCurrentDMXDevice(DMXDevice * d)
{
	if (dmxDevice.get() == d) return;

	if (dmxDevice != nullptr)
	{
		dmxDevice->removeDMXDeviceListener(this);
		dmxDevice->clearDevice();
		moduleParams.removeChildControllableContainer(dmxDevice.get());
	}

	dmxDevice.reset(d);
	
	dmxConnected->hideInEditor = dmxDevice == nullptr || dmxDevice->type == DMXDevice::ARTNET;
	dmxConnected->setValue(false);

	if (dmxDevice != nullptr)
	{
		dmxDevice->enabled = enabled->boolValue();
		dmxDevice->addDMXDeviceListener(this);
		moduleParams.addChildControllableContainer(dmxDevice.get());
		setupIOConfiguration(dmxDevice->canReceive && dmxDevice->inputCC->enabled->boolValue(), dmxDevice->outputCC->enabled->boolValue());
		dmxConnected->setValue(dmxDevice->isConnected);
	}


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
	for (int i = 0; i < numValues; ++i)
	{
		int value = values[i];
		dmxValues.set(i * 2, byteOrder == MSB ? (value >> 8) & 0xFF : value & 0xFF);
		dmxValues.set(i * 2 + 1, byteOrder == MSB ? 0xFF : (value >> 8) & 0xFF);
	}

	dmxDevice->sendDMXRange(startChannel, dmxValues);
}

var DMXModule::sendDMXFromScript(const var::NativeFunctionArgs& args)
{
	DMXModule * m = getObjectFromJS<DMXModule>(args);
	if (!m->enabled->boolValue()) return var();

	if (args.numArguments < 2) return var();

	int startChannel = args.arguments[0];
	Array<int> values;
	for (int i = 1; i < args.numArguments; ++i)
	{
		if (args.arguments[i].isArray())
		{
			for (int j = 0; j < args.arguments[i].size();j++) values.add(args.arguments[i][j]);
		}
		else
		{
			values.add(args.arguments[i]);
		}
	}

	m->sendDMXValues(startChannel, values);
	return var();

}

void DMXModule::clearItem()
{
	BaseItem::clearItem();
	setCurrentDMXDevice(nullptr);
}

var DMXModule::getJSONData()
{
	var data = Module::getJSONData();
	if (dmxDevice != nullptr) data.getDynamicObject()->setProperty("device", dmxDevice->getJSONData());
	
	var channelTypes;
	for (auto& v : channelValues)
	{
		if (v->type != DMXByteOrder::BIT8)
		{
			var vData(new DynamicObject());
			vData.getDynamicObject()->setProperty("channel", v->channel);
			vData.getDynamicObject()->setProperty("type",(int)v->type);
			channelTypes.append(vData);
		}
	}
	if(channelTypes.size() > 0) data.getDynamicObject()->setProperty("dmxChannelTypes", channelTypes);

	return data;
}

void DMXModule::loadJSONDataInternal(var data)
{
	Module::loadJSONDataInternal(data);
	if (dmxDevice != nullptr && data.getDynamicObject()->hasProperty("device")) dmxDevice->loadJSONData(data.getProperty("device", ""));

	var channelTypes = data.getProperty("dmxChannelTypes", var());
	for (int i = 0; i < channelTypes.size(); ++i)
	{
		channelValues[(int)channelTypes[i].getProperty("channel", 1) - 1]->setType((DMXByteOrder)(int)channelTypes[i].getProperty("type", 0));
	}

}


void DMXModule::onContainerParameterChanged(Parameter* p)
{
	Module::onContainerParameterChanged(p);
	if (p == enabled)
	{
		if (dmxDevice != nullptr) dmxDevice->enabled = enabled->boolValue();
	}
}

void DMXModule::controllableFeedbackUpdate(ControllableContainer * cc, Controllable * c)
{
	Module::controllableFeedbackUpdate(cc, c);
	if (c == dmxType) setCurrentDMXDevice(DMXDevice::create((DMXDevice::Type)(int)dmxType->getValueData()));
	else if (dmxDevice != nullptr)
	{
		if (c == dmxDevice->outputCC->enabled || (dmxDevice->canReceive && (c == dmxDevice->inputCC->enabled)))
		{
			setupIOConfiguration(dmxDevice->canReceive && dmxDevice->inputCC->enabled->boolValue(), dmxDevice->outputCC->enabled->boolValue());
		}
	}
}

void DMXModule::dmxDeviceConnected()
{
	dmxConnected->setValue(true);
}

void DMXModule::dmxDeviceDisconnected()
{
	dmxConnected->setValue(false);
}

void DMXModule::dmxDataInChanged(int numChannels, uint8* values)
{
	if (isClearing || !enabled->boolValue()) return;
	if (logIncomingData->boolValue()) NLOG(niceName, "DMX In : " + String(numChannels) + " channels received.");
	inActivityTrigger->trigger();

	var data;

	for (int i = 0; i < numChannels; ++i)
	{
		DMXValueParameter* vp = channelValues[i];
		if(vp->type == DMXByteOrder::BIT8) vp->setValue(values[i]);
		else if (i < numChannels - 1)
		{
			vp->setValueFrom2Channels(values[i], values[i+1]);
			i++;
		}

		data.append(vp->getValue());
	}

	scriptManager->callFunctionOnAllItems(dmxEventId, Array<var>{data});
}


DMXModule::DMXRouteParams::DMXRouteParams(Module * sourceModule, Controllable * c) :
	mode16bit(nullptr),
	fullRange(nullptr),
    channel(nullptr)
{
	channel = addIntParameter("Channel", "The Channel", 1, 1, 512);
	
	if (c->type == Controllable::FLOAT || c->type == Controllable::BOOL || c->type == Controllable::POINT2D || c->type == Controllable::POINT3D)
	{
		fullRange = addBoolParameter("Full Range", "If checked, value will be remapped from 0-1 will to 0-255 (or 0-65535 in 16-bit mode)", true);
	}
	
	if (c->type == Controllable::FLOAT || c->type == Controllable::INT || c->type == Controllable::BOOL || c->type == Controllable::POINT2D || c->type == Controllable::POINT3D || c->type == Controllable::COLOR)
	{
		mode16bit = addEnumParameter("Mode", "Choosing the resolution and Byte order for this routing");
		mode16bit->addOption("8-bit", BIT8)->addOption("16-bit MSB", MSB)->addOption("16-bit LSB", LSB);
	}
}

void DMXModule::handleRoutedModuleValue(Controllable * c, RouteParams * p)
{
	
	if (p == nullptr || c == nullptr) return;

	if (DMXRouteParams* rp = dynamic_cast<DMXRouteParams*>(p))
	{
		Parameter* sp = c->type == Controllable::TRIGGER ? nullptr : dynamic_cast<Parameter*>(c);

		bool fullRange = rp->fullRange != nullptr ? rp->fullRange->boolValue() : false;

		DMXByteOrder byteOrder = rp->mode16bit != nullptr ? rp->mode16bit->getValueDataAsEnum<DMXByteOrder>() : DMXByteOrder::BIT8;

		switch (c->type)
		{
		case Controllable::BOOL:
		case Controllable::INT:
		case Controllable::FLOAT:
			if (byteOrder == BIT8) sendDMXValue(rp->channel->intValue(), fullRange ? sp->getNormalizedValue() * 255 : (float)sp->getValue());
			else send16BitDMXValue(rp->channel->intValue(), fullRange ? sp->getNormalizedValue() * 65535 : (float)sp->getValue(), byteOrder);
			break;

		case Controllable::POINT2D:
		{
			Point<float> pp = ((Point2DParameter*)sp)->getPoint();
			if (fullRange) pp *= byteOrder != BIT8 ? 65535 : 255;

			Array<int> values;
			values.add((int)pp.x, (int)pp.y);

			if (byteOrder == BIT8) sendDMXValues(rp->channel->intValue(), values);
			else send16BitDMXValues(rp->channel->intValue(), values, byteOrder);
		}
		break;

		case Controllable::POINT3D:
		{
			Vector3D<float> pp = ((Point3DParameter*)sp)->getVector();
			if (fullRange) pp *= byteOrder != BIT8 ? 65535 : 255;

			Array<int> values;
			values.add((int)pp.x, (int)pp.y, (int)pp.z);

			if (byteOrder == BIT8) sendDMXValues(rp->channel->intValue(), values);
			else send16BitDMXValues(rp->channel->intValue(), values, byteOrder);
		}
		break;

		case Controllable::COLOR:
		{
			Colour col = ((ColorParameter*)sp)->getColor();
			Array<int> values;
			values.add(col.getRed(), col.getGreen(), col.getBlue());
			sendDMXValues(rp->channel->intValue(), values);
		}

		break;

		default:
			break;
		}
	}
}

DMXModule::DMXModuleRouterController::DMXModuleRouterController(ModuleRouter* router) :
	ModuleRouterController(router)
{
	autoSetChannels = addTrigger("Auto-set channels", "Auto set channels");
}

void DMXModule::DMXModuleRouterController::triggerTriggered(Trigger* t)
{
	if (t == autoSetChannels)
	{
		int startChannel = 0;
		for (auto& mrv : router->sourceValues.items)
		{
			if (DMXRouteParams* dp = dynamic_cast<DMXRouteParams*>(mrv->routeParams.get()))
			{
				dp->channel->setValue(startChannel++);
			}
		}
	}
}

ControllableUI * DMXValueParameter::createDefaultUI()
{
	return new DMXValueParameterUI(this);
}
