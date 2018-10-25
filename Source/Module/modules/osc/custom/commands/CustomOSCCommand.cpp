/*
  ==============================================================================

    CustomOSCCommand.cpp
    Created: 3 Nov 2016 12:41:23pm
    Author:  bkupe

  ==============================================================================
*/

#include "CustomOSCCommand.h"

CustomOSCCommand::CustomOSCCommand(CustomOSCModule * module, CommandContext context, var params) :
	OSCCommand(module, context, params),
	wildcardsContainer("Address Parameters")
{
	address->setControllableFeedbackOnly(false);
	address->isSavable = true;

	removeChildControllableContainer(&argumentsContainer);
	
	customValuesManager = new CustomValuesCommandArgumentManager(context == MAPPING);
	addChildControllableContainer(customValuesManager);  
	customValuesManager->addArgumentManagerListener(this);
}

CustomOSCCommand::~CustomOSCCommand()
{
	masterReference.clear();
}


void CustomOSCCommand::trigger()
{
	if (oscModule == nullptr) return;
	
	BaseCommand::trigger();

	String addString = address->stringValue();
	
	try
	{
		OSCMessage m(address->stringValue());

		for (auto &a : customValuesManager->items)
		{
			Parameter * p = a->param;
			if (p == nullptr) continue;
			switch (p->type)
			{
			case Controllable::BOOL: m.addInt32(p->boolValue() ? 1 : 0); break;
			case Controllable::INT: m.addInt32(p->intValue()); break;
			case Controllable::FLOAT: m.addFloat32(p->floatValue()); break;
			case Controllable::STRING: m.addString(p->stringValue()); break;
			case Controllable::COLOR:
			{
			Colour c = ((ColorParameter *)p)->getColor();
			m.addFloat32(c.getFloatRed());
			m.addFloat32(c.getFloatGreen());
			m.addFloat32(c.getFloatBlue());
			m.addFloat32(c.getFloatAlpha());
			}
			break;

			case Controllable::POINT2D:
				m.addFloat32(((Point2DParameter *)p)->x);
				m.addFloat32(((Point2DParameter *)p)->y);
				break;
			case Controllable::POINT3D:
				m.addFloat32(((Point3DParameter *)p)->x);
				m.addFloat32(((Point3DParameter *)p)->y);
				m.addFloat32(((Point3DParameter *)p)->z);
				break;

			default:
				//not handle
				break;

			}
		}
		oscModule->sendOSC(m);
	}catch (const OSCFormatError &)
	{
		NLOG("OSC", "Address is invalid :\n" << address->stringValue());
		return;
	}
}

void CustomOSCCommand::useForMappingChanged(CustomValuesCommandArgument *)
{
	if (context != MAPPING) return;

	clearTargetMappingParameters();
	int index = 0;
	for (auto &a : customValuesManager->items)
	{
		if (a->useForMapping->boolValue())
		{
			setTargetMappingParameterAt(a->param, index);
			index++;
		}
	}
}

void CustomOSCCommand::onContainerParameterChanged(Parameter * p)
{
	OSCCommand::onContainerParameterChanged(p);
	if (p == address)
	{


		if (wildcardsContainer.items.size() == 0)
		{
			if (wildcardsContainer.parentContainer == this) removeChildControllableContainer(&wildcardsContainer);
		} else
		{
			if (wildcardsContainer.parentContainer != this) addChildControllableContainer(&wildcardsContainer);
		}
	}
}


var CustomOSCCommand::getJSONData()
{
	var data = OSCCommand::getJSONData();
	data.getDynamicObject()->setProperty("argManager", customValuesManager->getJSONData());
	return data;
}

void CustomOSCCommand::loadJSONDataInternal(var data)
{
	OSCCommand::loadJSONDataInternal(data);
	customValuesManager->loadJSONData(data.getProperty("argManager", var()), true);
}