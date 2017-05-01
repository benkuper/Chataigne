/*
  ==============================================================================

    DMXModule.cpp
    Created: 6 Apr 2017 10:22:10pm
    Author:  Ben

  ==============================================================================
*/

#include "DMXModule.h"
#include "DMXCommand.h"
#include "DMXModuleEditor.h"

DMXModule::DMXModule() :
	Module("DMX"),
	dmxDevice(nullptr)
{
	//canHandleRouteValues = true;

	defManager.add(CommandDefinition::createDef(this, "", "Set value", &DMXCommand::create)->addParam("action", DMXCommand::SET_VALUE));
	defManager.add(CommandDefinition::createDef(this, "", "Set range", &DMXCommand::create)->addParam("action", DMXCommand::SET_RANGE));
	defManager.add(CommandDefinition::createDef(this, "", "Set Color", &DMXCommand::create)->addParam("action",DMXCommand::COLOR));
	defManager.add(CommandDefinition::createDef(this, "", "Clear all", &DMXCommand::create)->addParam("action", DMXCommand::CLEAR_ALL));
	dmxType = addEnumParameter("DMX Type", "Choose the type of dmx interface you want to connect");

	dmxType->addOption("Open DMX", DMXDevice::OPENDMX);
	dmxType->addOption("Enttec DMX Pro", DMXDevice::ENTTEC_DMXPRO);
	dmxType->addOption("Enttec DMX MkII", DMXDevice::ENTTEC_MK2);
	dmxType->addOption("Art-Net", DMXDevice::ARTNET);

	dmxType->setValueWithKey("Open DMX");

	dmxConnected = addBoolParameter("Connected", "DMX is connected ?", false);

	for (int i = 1; i <= 512; i++)
	{
		dmxInValues.add(valuesCC.addIntParameter("Channel " + String(i), "DMX Value for channel " + String(i), 0, 0, 255));
	}

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
		removeChildControllableContainer(dmxDevice);
	}

	dmxDevice = d;

	if (dmxDevice != nullptr)
	{
		dmxDevice->addDMXDeviceListener(this);
		addChildControllableContainer(dmxDevice);
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

void DMXModule::onContainerParameterChangedInternal(Parameter * p)
{
	Module::onContainerParameterChangedInternal(p);

	if (p == dmxType)
	{
		setCurrentDMXDevice(DMXDevice::create((DMXDevice::Type)(int)dmxType->getValueData()));
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

void DMXModule::dmxDataInChanged(int channel, int value)
{
	if (logIncomingData->boolValue()) NLOG(niceName, "DMX In : " + String(channel) + " > " + String(value));
	inActivityTrigger->trigger();

	dmxInValues[channel - 1]->setValue(value);
}

InspectableEditor * DMXModule::getEditor(bool isRoot)
{
	return new DMXModuleEditor(this,isRoot);
}

