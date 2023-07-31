/*
  ==============================================================================

	OSCCommand.cpp
	Created: 3 Nov 2016 10:47:46am
	Author:  bkupe

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

OSCCommand::OSCCommand(IOSCSenderModule* _module, CommandContext context, var params, Multiplex* multiplex) :
	BaseCommand(dynamic_cast<Module*>(_module), context, params, multiplex),
	oscModule(_module),
	argumentsContainer("Arguments", multiplex)
{
	address = addStringParameter("Address", "Adress of the OSC Message (e.g. /example)", params.getProperty("address", "/example"));
	address->setControllableFeedbackOnly(true);
	address->isSavable = false;
	argumentsContainer.addControllableContainerListener(this);
	addChildControllableContainer(&argumentsContainer);

	addressModel = address->stringValue();
	rebuildAddressOnParamChanged = address->stringValue().containsChar('[');

	buildArgsAndParamsFromData(params);

	if (rebuildAddressOnParamChanged) rebuildAddress();
}

OSCCommand::~OSCCommand()
{
}

void OSCCommand::rebuildAddress()
{
	address->setValue(getTargetAddress());
}

String OSCCommand::getTargetAddress(int multiplexIndex)
{
	String targetAddress(addressModel);
	if (targetAddress.containsChar('['))
	{
		//rebuild by replacing [..] with parameters
		for (auto& c : controllables)
		{
			if (c->type != Controllable::TRIGGER && c != address)
			{
				Parameter* p = static_cast<Parameter*>(c);
				if (p == nullptr) continue;

				targetAddress = targetAddress.replace("[" + p->shortName + "]", getLinkedValue(p, multiplexIndex).toString());
			}
		}
	}

	String result = getTargetAddressInternal(targetAddress, multiplexIndex);
	if (result.isEmpty()) return getLinkedValue(address, multiplexIndex).toString();

	return result;
}

void OSCCommand::buildArgsAndParamsFromData(var data)
{
	if (data.getDynamicObject()->hasProperty("args"))
	{
		if (data.getProperty("args", var()).isArray())
		{
			Array<var>* argsArray = data.getProperty("args", var()).getArray();
			for (auto& a : *argsArray)
			{
				Parameter* p = static_cast<Parameter*>(ControllableFactory::createControllable(a.getProperty("type", "")));
				if (p == nullptr) continue;
				p->saveValueOnly = false;
				p->loadJSONData(a);
				argumentsContainer.addParameter(p);
				if (a.hasProperty("mappingIndex")) argumentsContainer.linkParamToMappingIndex(p, a.getProperty("mappingIndex", 0));

				if (p->type == Controllable::ENUM && a.hasProperty("options"))
				{
					EnumParameter* ep = (EnumParameter*)p;
					NamedValueSet optionsData = a.getProperty("options", var()).getDynamicObject()->getProperties();
					for (auto& o : optionsData) ep->addOption(o.name.toString(), o.value);
				}
			}
		}

		argumentsContainer.hideInEditor = argumentsContainer.controllables.size() == 0;
	}

	argumentsContainer.hideInEditor = data.getProperty("hideArgs", argumentsContainer.hideInEditor);

	if (data.getDynamicObject()->hasProperty("params") && data.getProperty("params", var()).isArray())
	{
		Array<var>* argsArray = data.getProperty("params", var()).getArray();
		for (auto& a : *argsArray)
		{
			Parameter* p = static_cast<Parameter*>(ControllableFactory::createControllable(a.getProperty("type", "")));
			if (p == nullptr) continue;
			p->saveValueOnly = false;
			p->loadJSONData(a);
			addParameter(p);
		}
	}
}

var OSCCommand::getJSONData()
{
	var data = BaseCommand::getJSONData();
	data.getDynamicObject()->setProperty("arguments", argumentsContainer.getJSONData());
	return data;
}

void OSCCommand::loadJSONDataInternal(var data)
{
	BaseCommand::loadJSONDataInternal(data);
	argumentsContainer.loadJSONData(data.getProperty("arguments", var()), true);
}

void OSCCommand::controllableAdded(Controllable* c)
{
	if (c->parentContainer == &argumentsContainer)
	{
		onControllableAdded(c);
	}
}

void OSCCommand::onContainerParameterChanged(Parameter* p)
{
	if (p != address && rebuildAddressOnParamChanged)
	{
		rebuildAddress();
	}
}

void OSCCommand::updateMappingInputValue(var value, int multiplexIndex)
{
	BaseCommand::updateMappingInputValue(value, multiplexIndex);
	for (auto& a : argumentsContainer.paramLinks) a->updateMappingInputValue(value, multiplexIndex);
}

void OSCCommand::setInputNamesFromParams(Array<WeakReference<Parameter>> outParams)
{
	BaseCommand::setInputNamesFromParams(outParams);
	argumentsContainer.setInputNamesFromParams(outParams);
}

void OSCCommand::triggerInternal(int multiplexIndex)
{
	if (oscModule == nullptr) return;

	BaseCommand::triggerInternal(multiplexIndex);
	String addrString = getTargetAddress(multiplexIndex); //forces iteratives to reevalute the address

	try
	{
		OSCMessage m(addrString);

		for (auto& a : argumentsContainer.controllables)
		{
			if (!a->enabled) continue;

			Parameter* p = static_cast<Parameter*>(a);
			if (p == nullptr) continue;

			var val = argumentsContainer.getLinkedValue(p, multiplexIndex);
			OSCHelpers::addArgumentsForParameter(m, p, oscModule->getBoolMode(), oscModule->getColorMode(), val);
		}

		oscModule->sendOSC(m);
	}
	catch (OSCFormatError& e)
	{
		LOGERROR("Can't send to address " << addrString << " : " << e.description);
	}
}
