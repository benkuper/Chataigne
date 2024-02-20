
/*
  ==============================================================================

	DMXModule.cpp
	Created: 6 Apr 2017 10:22:10pm
	Author:  Ben

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

DMXModule::DMXModule() :
	Module("DMX"),
	Thread("DMX Send"),
	dmxDevice(nullptr),
	inputUniverseManager(true),
	outputUniverseManager(false)
{
	outputUniverseManager.setNiceName("Output Universes");
	setupIOConfiguration(false, true);

	valuesCC.editorIsCollapsed = true;
	includeValuesInSave = false;
	canHandleRouteValues = true;

	defManager->add(CommandDefinition::createDef(this, "", "Black out", &DMXCommand::create)->addParam("action", DMXCommand::BLACK_OUT));
	defManager->add(CommandDefinition::createDef(this, "", "Set value", &DMXCommand::create)->addParam("action", DMXCommand::SET_VALUE));
	defManager->add(CommandDefinition::createDef(this, "", "Set value 16bit", &DMXCommand::create)->addParam("action", DMXCommand::SET_VALUE_16BIT));
	defManager->add(CommandDefinition::createDef(this, "", "Set range", &DMXCommand::create)->addParam("action", DMXCommand::SET_RANGE));
	defManager->add(CommandDefinition::createDef(this, "", "Set all", &DMXCommand::create)->addParam("action", DMXCommand::SET_ALL));
	defManager->add(CommandDefinition::createDef(this, "", "Set custom values", &DMXCommand::create)->addParam("action", DMXCommand::SET_CUSTOM));
	defManager->add(CommandDefinition::createDef(this, "", "Set Color", &DMXCommand::create)->addParam("action", DMXCommand::COLOR));
	defManager->add(CommandDefinition::createDef(this, "", "Set Color 16bit", &DMXCommand::create)->addParam("action", DMXCommand::COLOR_16BIT));

	dmxType = moduleParams.addEnumParameter("DMX Type", "Choose the type of dmx interface you want to connect");

	dmxType->addOption("Open DMX", DMXDevice::OPENDMX)->addOption("Enttec DMX Pro", DMXDevice::ENTTEC_DMXPRO)->addOption("Enttec DMX MkII", DMXDevice::ENTTEC_MK2)->addOption("Art-Net", DMXDevice::ARTNET)->addOption("sACN/E1.31", DMXDevice::SACN);
	dmxType->setValueWithKey("Open DMX");

	sendRate = moduleParams.addIntParameter("Send Rate", "The rate at which to send data.", 40, 1, 200);
	sendOnChangeOnly = moduleParams.addBoolParameter("Send On Change Only", "Only send a universe if one of its channels has changed", false);
	useMulticast = moduleParams.addBoolParameter("Use Multicast", "Use Multicast on receive and send if applicable with the device type", false);

	autoAdd = moduleParams.addBoolParameter("Auto Add", "If checked, received universed will automatically be added to the values. Not effective when using 1-universe devices like OpenDMX or Enttec DMXPro", true);



	moduleParams.addChildControllableContainer(&outputUniverseManager);
	valuesCC.addChildControllableContainer(&inputUniverseManager);

	if (!Engine::mainEngine->isLoadingFile)
	{
		outputUniverseManager.addItem();
	}

	//valuesCC.userCanAddControllables = true;
	//valuesCC.customUserCreateControllableFunc = &DMXModule::showMenuAndCreateValue;

	setCurrentDMXDevice(DMXDevice::create((DMXDevice::Type)(int)dmxType->getValueData()));

	//Script
	scriptObject.getDynamicObject()->setMethod(sendDMXId, DMXModule::sendDMXFromScript);
	scriptObject.getDynamicObject()->setMethod(sendDMXUniverseId, DMXModule::sendDMXUniverseFromScript);

	//for (int i = 0; i < 512; ++i)
	//{
	//	int channel = i + 1;
	//	DMXValueParameter* dVal = new DMXValueParameter("Channel " + String(channel), "DMX Value for channel " + String(channel), channel, 0, DMXByteOrder::BIT8);

	//	valuesCC.addParameter(dVal);
	//	channelValues.add(dVal);
	//}


	thruManager.reset(new ControllableContainer("Pass-through"));
	thruManager->userCanAddControllables = true;
	thruManager->customUserCreateControllableFunc = &DMXModule::createThruControllable;
	moduleParams.addChildControllableContainer(thruManager.get());

	inputUniverseManager.addBaseManagerListener(this);
	outputUniverseManager.addBaseManagerListener(this);
}

DMXModule::~DMXModule()
{
}

void DMXModule::itemAdded(DMXUniverseItem* i)
{
	updateDeviceMulticast();
}

void DMXModule::itemsAdded(Array<DMXUniverseItem*> items)
{
	updateDeviceMulticast();
}

void DMXModule::itemRemoved(DMXUniverseItem* i)
{
	updateDeviceMulticast();
}

void DMXModule::itemsRemoved(Array<DMXUniverseItem*> items)
{
	updateDeviceMulticast();
}

void DMXModule::setCurrentDMXDevice(DMXDevice* d)
{
	if (dmxDevice.get() == d) return;

	stopThread(1000);

	if (dmxDevice != nullptr)
	{
		dmxDevice->removeDMXDeviceListener(this);
		dmxDevice->clearDevice();
		moduleParams.removeChildControllableContainer(dmxDevice.get());
		connectionFeedbackRef = nullptr;
	}

	dmxDevice.reset(d);

	if (dmxDevice != nullptr)
	{
		dmxDevice->setEnabled(enabled->boolValue());
		dmxDevice->addDMXDeviceListener(this);
		moduleParams.addChildControllableContainer(dmxDevice.get(), false, 0);
		connectionFeedbackRef = dmxDevice->isConnected;
		setupIOConfiguration(dmxDevice->canReceive && dmxDevice->inputCC->enabled->boolValue(), dmxDevice->outputCC->enabled->boolValue());

		useMulticast->setEnabled(dmxDevice->type == DMXDevice::SACN);

		updateDeviceMulticast();
		inputUniverseManager.setFirstUniverse(dmxDevice->getFirstUniverse());
		outputUniverseManager.setFirstUniverse(dmxDevice->getFirstUniverse());


		if (!isCurrentlyLoadingData) startThread();
	}
	else
	{
		useMulticast->setEnabled(false);
	}


	dmxModuleListeners.call(&DMXModuleListener::dmxDeviceChanged);
}

void DMXModule::updateDeviceMulticast()
{
	if (isCurrentlyLoadingData) return;
	if (dmxDevice == nullptr) return;

	Array<DMXUniverse*> inUniv;
	Array<DMXUniverse*> outUniv;

	if (useMulticast->boolValue())
	{
		inUniv = Array<DMXUniverse*>(inputUniverseManager.items.getRawDataPointer(), inputUniverseManager.items.size());
		outUniv = Array<DMXUniverse*>(outputUniverseManager.items.getRawDataPointer(), outputUniverseManager.items.size());
	}

	dmxDevice->setupMulticast(inUniv, outUniv);
}

void DMXModule::sendDMXValue(DMXUniverse* u, int channel, uint8 value)
{
	if (!enabled->boolValue()) return;
	if (dmxDevice == nullptr) return;
	if (u == nullptr) return;

	if (channel <= 0 || channel > DMX_NUM_CHANNELS) return;

	if (logOutgoingData->boolValue()) NLOG(niceName, "Send DMX Channel " << channel << ", Value " << value << " to " << u->toString());
	outActivityTrigger->trigger();

	channel--; //rebase at 0

	u->updateValue(channel, value);
}

void DMXModule::sendDMXRange(DMXUniverse* u, int startChannel, Array<uint8> values)
{
	if (!enabled->boolValue()) return;
	if (dmxDevice == nullptr) return;
	if (u == nullptr) return;

	if (startChannel <= 0) return;

	if (logOutgoingData->boolValue())NLOG(niceName, "Send DMX Range " << startChannel << " > " << startChannel + values.size() - 1 << " to " << u->toString());

	outActivityTrigger->trigger();

	startChannel--; //rebase at 0


	for (int i = startChannel; i < startChannel + values.size() && i < DMX_NUM_CHANNELS; i++)
	{
		u->updateValue(i, values[i - startChannel]);
	}
}

void DMXModule::send16BitDMXValue(DMXUniverse* u, int channel, int value, DMXByteOrder byteOrder)
{
	if (!enabled->boolValue()) return;
	if (dmxDevice == nullptr) return;
	if (u == nullptr) return;

	if (channel <= 0 || channel >= DMX_NUM_CHANNELS) return;

	if (logOutgoingData->boolValue()) NLOG(niceName, "Send 16-bit DMX Channel " << channel << ", Value " << value << " to " << u->toString());
	outActivityTrigger->trigger();

	channel--; //rebase at 0

	u->updateValue(channel, byteOrder == MSB ? (value >> 8) & 0xFF : value & 0xFF);
	u->updateValue(channel + 1, byteOrder == MSB ? value & 0xFF : (value >> 8) & 0xFF);
}

void DMXModule::send16BitDMXRange(DMXUniverse* u, int startChannel, Array<int> values, DMXByteOrder byteOrder)
{
	if (!enabled->boolValue()) return;
	if (dmxDevice == nullptr) return;
	if (u == nullptr) return;

	if (startChannel <= 0) return;

	if (logOutgoingData->boolValue()) NLOG(niceName, "Send 16-bit DMX Range " << startChannel << " > " << startChannel + values.size() - 1 << " to " << u->toString());
	outActivityTrigger->trigger();

	startChannel--; //rebase at 0

	for (int i = 0; i < values.size(); ++i)
	{
		int index = startChannel + i * 2;
		if (index >= DMX_NUM_CHANNELS - 1) break;

		int value = values[i];

		u->updateValue(index, byteOrder == MSB ? (value >> 8) & 0xFF : value & 0xFF);
		u->updateValue(index + 1, byteOrder == MSB ? value & 0xFF : (value >> 8) & 0xFF);
	}
}

var DMXModule::sendDMXFromScript(const var::NativeFunctionArgs& args)
{
	DMXModule* m = getObjectFromJS<DMXModule>(args);
	if (!m->enabled->boolValue()) return var();

	if (args.numArguments < 2) return var();

	int startChannel = args.arguments[0];
	Array<uint8> values;
	for (int i = 1; i < args.numArguments; ++i)
	{
		if (args.arguments[i].isArray())
		{
			for (int j = 0; j < args.arguments[i].size(); j++) values.add((int)args.arguments[i][j]);
		}
		else
		{
			values.add((int)args.arguments[i]);
		}
	}

	DMXUniverse* u = m->getUniverse(false, 0, 0, 0, false);
	if (u != nullptr) m->sendDMXRange(u, startChannel, values);

	return var();

}

var DMXModule::sendDMXUniverseFromScript(const var::NativeFunctionArgs& args)
{
	DMXModule* m = getObjectFromJS<DMXModule>(args);
	if (!m->enabled->boolValue()) return var();

	if (args.numArguments < 5) return var();

	int net = args.arguments[0];
	int subnet = args.arguments[1];
	int universe = args.arguments[2];
	int startChannel = args.arguments[3];

	Array<uint8> values;
	for (int i = 4; i < args.numArguments; ++i)
	{
		if (args.arguments[i].isArray())
		{
			for (int j = 0; j < args.arguments[i].size(); j++) values.add((int)args.arguments[i][j]);
		}
		else
		{
			values.add((int)args.arguments[i]);
		}
	}

	DMXUniverse* u = m->getUniverse(false, net, subnet, universe, false);
	if (u != nullptr) m->sendDMXRange(u, startChannel, values);

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
			vData.getDynamicObject()->setProperty("type", (int)v->type);
			channelTypes.append(vData);
		}
	}
	if (channelTypes.size() > 0) data.getDynamicObject()->setProperty("dmxChannelTypes", channelTypes);
	data.getDynamicObject()->setProperty(inputUniverseManager.shortName, inputUniverseManager.getJSONData());

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

	inputUniverseManager.loadJSONData(data.getProperty(inputUniverseManager.shortName, var()));

	if (thruManager != nullptr)
	{
		//thruManager->loadJSONData(data.getProperty("thru", var()));
		for (auto& c : thruManager->controllables)
		{
			if (TargetParameter* mt = dynamic_cast<TargetParameter*>(c))
			{
				mt->targetType = TargetParameter::CONTAINER;
				mt->customGetTargetContainerFunc = &ModuleManager::showAndGetModuleOfType<DMXModule>;
				mt->isRemovableByUser = true;
				mt->canBeDisabledByUser = true;
			}
		}
	}
}

void DMXModule::afterLoadJSONDataInternal()
{
	Module::afterLoadJSONDataInternal();
	updateDeviceMulticast();
	if (dmxDevice != nullptr) startThread();
}


void DMXModule::onContainerParameterChanged(Parameter* p)
{
	Module::onContainerParameterChanged(p);
	if (p == enabled)
	{
		if (dmxDevice != nullptr) {
			dmxDevice->setEnabled(enabled->boolValue());
		}

		if (enabled->boolValue())
		{
			if (dmxDevice != nullptr && !isCurrentlyLoadingData) startThread();
		}
		else stopThread(1000);
	}
}

void DMXModule::controllableFeedbackUpdate(ControllableContainer* cc, Controllable* c)
{
	Module::controllableFeedbackUpdate(cc, c);
	if (c == dmxType) setCurrentDMXDevice(DMXDevice::create((DMXDevice::Type)(int)dmxType->getValueData()));
	else if (dmxDevice != nullptr)
	{
		if (c == dmxDevice->outputCC->enabled || (dmxDevice->canReceive && (c == dmxDevice->inputCC->enabled)))
		{
			setupIOConfiguration(dmxDevice->canReceive && dmxDevice->inputCC->enabled->boolValue(), dmxDevice->outputCC->enabled->boolValue());

		}
		else if (c == useMulticast)
		{
			updateDeviceMulticast();
		}
	}
}

void DMXModule::dmxDeviceSetupChanged(DMXDevice*)
{
	connectionFeedbackRef = dmxDevice->isConnected;

}
void DMXModule::dmxDataInChanged(DMXDevice*, int net, int subnet, int universe, int priority, Array<uint8> values, const String& sourceName)
{
	if (isClearing || !enabled->boolValue()) return;
	if (logIncomingData->boolValue())
	{
		String s = "DMX In received :\nNet : " + String(net) + "\nSubnet : " + String(subnet) + "\nUniverse : " + String(universe);
		if (sourceName.isNotEmpty()) s += " from " + sourceName;
		NLOG(niceName, s);
	}

	inActivityTrigger->trigger();


	if (thruManager != nullptr)
	{
		for (auto& c : thruManager->controllables)
		{
			if (TargetParameter* mt = (TargetParameter*)c)
			{
				if (!mt->enabled) continue;
				if (DMXModule* m = (DMXModule*)(mt->targetContainer.get()))
				{
					if (m->dmxDevice != nullptr)
					{
						m->dmxDevice->sendDMXValues(net, subnet, universe, priority, values.getRawDataPointer());
					}
				}
			}
		}
	}

	DMXUniverse* u = getUniverse(true, net, subnet, universe, autoAdd->boolValue());
	if (u == nullptr) return;

	u->updateValues(values);

	if (scriptManager->items.size() > 0)
	{
		Array<var> args;
		args.add(net);
		args.add(subnet);
		args.add(universe);
		var data;
		for (auto& v : values) data.append(v);
		args.add(data);
		scriptManager->callFunctionOnAllItems(dmxEventId, args);
	}
}

DMXUniverse* DMXModule::getUniverse(bool isInput, int net, int subnet, int universe, int priority, bool createIfNotThere)
{
	DMXUniverseManager* m = isInput ? &inputUniverseManager : &outputUniverseManager;
	for (auto& u : m->items) if (u->checkSignature(net, subnet, universe)) return u;

	if (!createIfNotThere) return nullptr;

	DMXUniverseItem* u = new DMXUniverseItem(isInput);
	u->netParam->setValue(net);
	u->subnetParam->setValue(subnet);
	u->universeParam->setValue(universe);
	u->priorityParam->setValue(priority);

	return m->addItem(u);
}

void DMXModule::run()
{
	while (!threadShouldExit())
	{
		double t1 = Time::getMillisecondCounterHiRes();
		{
			GenericScopedLock lock(deviceLock);
			if (dmxDevice == nullptr) return;

			bool sendOnChange = sendOnChangeOnly->boolValue();
			for (auto& u : outputUniverseManager.items)
			{
				if (sendOnChange && !u->isDirty) continue;
				dmxDevice->sendDMXValues(u);
				u->isDirty = false;
			}
		}
		double t2 = Time::getMillisecondCounterHiRes();

		double diffTime = t2 - t1;
		double rateMS = 1000.0 / sendRate->intValue();

		double msToWait = rateMS - diffTime;
		if (msToWait > 0) wait(msToWait);

	}
}

void DMXModule::createThruControllable(ControllableContainer* cc)
{
	TargetParameter* p = new TargetParameter(cc->getUniqueNameInContainer("Output module 1"), "Target module to send the raw data to", "");
	p->targetType = TargetParameter::CONTAINER;
	p->customGetTargetContainerFunc = &ModuleManager::showAndGetModuleOfType<DMXModule>;
	p->isRemovableByUser = true;
	p->canBeDisabledByUser = true;
	p->saveValueOnly = false;
	cc->addParameter(p);
}


DMXModule::DMXRouteParams::DMXRouteParams(Module* sourceModule, Controllable* c, DMXUniverseManager& outputUniverseManager) :
	mode16bit(nullptr),
	fullRange(nullptr),
	channel(nullptr),
	dmxUniverse(nullptr)
{
	channel = addIntParameter("Channel", "The Channel", 1, 1, 512);

	dmxUniverse = addTargetParameter("Universe", "The Universe to use, you can create multiple ones in the Module Parameters", &outputUniverseManager);
	dmxUniverse->targetType = TargetParameter::CONTAINER;
	dmxUniverse->maxDefaultSearchLevel = 0;
	dmxUniverse->showParentNameInEditor = false;


	if (c->type == Controllable::FLOAT || c->type == Controllable::BOOL || c->type == Controllable::INT || c->type == Controllable::POINT2D || c->type == Controllable::POINT3D)
	{
		fullRange = addBoolParameter("Full Range", "If checked, value will be remapped from 0-1 will to 0-255 (or 0-65535 in 16-bit mode)", ((Parameter*)c)->hasRange());
	}

	if (c->type == Controllable::FLOAT || c->type == Controllable::INT || c->type == Controllable::BOOL || c->type == Controllable::POINT2D || c->type == Controllable::POINT3D || c->type == Controllable::COLOR)
	{
		mode16bit = addEnumParameter("Mode", "Choosing the resolution and Byte order for this routing");
		mode16bit->addOption("8-bit", BIT8)->addOption("16-bit MSB", MSB)->addOption("16-bit LSB", LSB);
	}
}

void DMXModule::handleRoutedModuleValue(Controllable* c, RouteParams* p)
{
	if (p == nullptr || c == nullptr) return;

	if (DMXRouteParams* rp = dynamic_cast<DMXRouteParams*>(p))
	{
		DMXUniverse* u = dynamic_cast<DMXUniverse*>(rp->dmxUniverse->targetContainer.get());

		Parameter* sp = c->type == Controllable::TRIGGER ? nullptr : dynamic_cast<Parameter*>(c);

		bool fullRange = rp->fullRange != nullptr ? rp->fullRange->boolValue() : false;

		DMXByteOrder byteOrder = rp->mode16bit != nullptr ? rp->mode16bit->getValueDataAsEnum<DMXByteOrder>() : DMXByteOrder::BIT8;

		if (sp == nullptr) return;

		switch (c->type)
		{
		case Parameter::BOOL:
		case Parameter::INT:
		case Parameter::FLOAT:
		{
			int value = (sp->hasRange() ? (float)sp->getNormalizedValue() : sp->floatValue()) * (fullRange ? (byteOrder == BIT8 ? 255 : 65535) : 1);

			if (byteOrder == BIT8) sendDMXValue(u, rp->channel->intValue(), value);
			else send16BitDMXValue(u, rp->channel->intValue(), value, byteOrder);
		}
		break;

		/*case Parameter::POINT2D:
		{
			Point<float> pp = ((Point2DParameter*)sp)->getPoint();
			if (fullRange) pp *= byteOrder != BIT8 ? 65535 : 255;

			Array<int> values;
			values.add((int)pp.x, (int)pp.y);

			if (byteOrder == BIT8) sendDMXValues(u, rp->channel->intValue(), values);
			else send16BitDMXValues(u, rp->channel->intValue(), values, byteOrder);
		}
		break;

		case Parameter::POINT3D:
		{
			Vector3D<float> pp = ((Point3DParameter*)sp)->getVector();
			if (fullRange) pp *= byteOrder != BIT8 ? 65535 : 255;

			Array<int> values;
			values.add((int)pp.x, (int)pp.y, (int)pp.z);

			if (byteOrder == BIT8) sendDMXValues(u, rp->channel->intValue(), values);
			else send16BitDMXValues(u, rp->channel->intValue(), values, byteOrder);
		}
		break;

		case Parameter::COLOR:
		{
			Colour col = ((ColorParameter*)sp)->getColor();
			Array<int> values;
			values.add(col.getRed(), col.getGreen(), col.getBlue());
			sendDMXValues(u, rp->channel->intValue(), values);
		}

		break;*/

		default:
			break;
		}
	}
}

DMXModule::DMXModuleRouterController::DMXModuleRouterController(ModuleRouter* router) :
	ModuleRouterController(router)
{
	autoSetChannels = addTrigger("Auto-set channels", "Auto set channels");

	autoSetUniverse = addTrigger("Auto-set universes", "Set the first universe of the DMX Module to all");
}

void DMXModule::DMXModuleRouterController::triggerTriggered(Trigger* t)
{
	if (t == autoSetChannels)
	{
		int startChannel = 1;
		for (auto& mrv : router->sourceValues.items)
		{
			if (DMXRouteParams* dp = dynamic_cast<DMXRouteParams*>(mrv->routeParams.get()))
			{
				dp->channel->setValue(startChannel++);
			}
		}
	}

	if (t == autoSetUniverse)
	{
		for (auto& mrv : router->sourceValues.items)
		{
			if (DMXRouteParams* dp = dynamic_cast<DMXRouteParams*>(mrv->routeParams.get()))
			{
				DMXUniverseManager* universeManager = dynamic_cast<DMXUniverseManager*>(dp->dmxUniverse->rootContainer.get());
				if (universeManager->items.size() > 0) dp->dmxUniverse->setValueFromTarget(universeManager->items[0]);
			}
		}
	}
}
