/*
  ==============================================================================

    OSCCommand.cpp
    Created: 3 Nov 2016 10:47:46am
    Author:  bkupe

  ==============================================================================
*/

#include "OSCCommand.h"
#include "OSCModule.h"

OSCCommand::OSCCommand(OSCModule * _module, CommandContext context, var params) :
	BaseCommand(_module,context, params),
	oscModule(_module),
	argumentsContainer("Arguments")
{
	address = addStringParameter("Address", "Adress of the OSC Message (e.g. /example)", params.getProperty("address","/example"));
	address->isEditable = false;
	addChildControllableContainer(&argumentsContainer);
	
	addressModel = address->stringValue();
	rebuildAddressOnParamChanged = address->stringValue().containsChar('[');

	buildArgsAndParamsFromData(params);
	
	if(rebuildAddressOnParamChanged) rebuildAddress();

	argumentsContainer.hideInEditor = argumentsContainer.controllables.size() == 0;
	
}

OSCCommand::~OSCCommand()
{
}

void OSCCommand::rebuildAddress()
{
	//rebuild by replacing [..] with parameters

	
	String targetAddress(addressModel);
	for (auto & c : controllables)
	{
		if (c->type != Controllable::TRIGGER && c != address)
		{
			Parameter * p = static_cast<Parameter *>(c);
			if (p == nullptr) continue;
			

			targetAddress = targetAddress.replace("[" + p->shortName + "]", p->stringValue());
		}
	}

	address->setValue(targetAddress);
}

void OSCCommand::buildArgsAndParamsFromData(var data)
{
	if (data.getDynamicObject()->hasProperty("args") && data.getProperty("args",var()).isArray())
	{
		Array<var>* argsArray = data.getProperty("args", var()).getArray();
		for (auto & a : *argsArray)
		{
			Parameter * p = static_cast<Parameter *>(ControllableFactory::createControllable(a.getProperty("type", "")));
			if (p == nullptr) continue;
			p->saveValueOnly = false;
			p->loadJSONData(a);
			argumentsContainer.addParameter(p);
			if (a.getDynamicObject()->hasProperty("mappingIndex")) setTargetMappingParameterAt(p, a.getProperty("mappingIndex", 0));

		}
	}

	if (data.getDynamicObject()->hasProperty("params") && data.getProperty("params",var()).isArray())
	{
		Array<var>* argsArray = data.getProperty("params", var()).getArray();
		for (auto & a : *argsArray)
		{
			Parameter * p = static_cast<Parameter *>(ControllableFactory::createControllable(a.getProperty("type", "")));
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

void OSCCommand::onContainerParameterChanged(Parameter * p)
{
	if (p != address && rebuildAddressOnParamChanged)
	{
		rebuildAddress();
	}
}

void OSCCommand::trigger()
{
	if (oscModule == nullptr) return;

	OSCMessage m(address->stringValue());
	
	for (auto &a : argumentsContainer.controllables)
	{
		Parameter * p = static_cast<Parameter *>(a);
		if (p == nullptr) continue;
		switch (p->type)
		{
		case Controllable::BOOL: m.addInt32(p->boolValue() ? 1 : 0); break;
		case Controllable::INT: m.addInt32(p->intValue()); break;
		case Controllable::FLOAT: m.addFloat32(p->floatValue()); break;
		case Controllable::STRING: m.addString(p->stringValue()); break;
		case Controllable::POINT2D:
			m.addFloat32(((Point2DParameter *)a)->x);
			m.addFloat32(((Point2DParameter *)a)->y);
			break;
		case Controllable::POINT3D:
			m.addFloat32(((Point3DParameter *)a)->x);
			m.addFloat32(((Point3DParameter *)a)->y);
			m.addFloat32(((Point3DParameter *)a)->z);
			break;
			
		default:
		 //not handle
		 break;

		}
	}

	oscModule->sendOSC(m);
}
