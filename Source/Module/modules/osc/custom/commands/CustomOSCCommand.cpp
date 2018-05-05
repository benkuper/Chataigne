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
	argManager(context == MAPPING)
{
	address->isEditable = true;
	address->isSavable = true;

	removeChildControllableContainer(&argumentsContainer);
	addChildControllableContainer(&argManager);  
	argManager.addArgumentManagerListener(this);
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

		for (auto &a : argManager.items)
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
	for (auto &a : argManager.items)
	{
		if (a->useForMapping->boolValue())
		{
			setTargetMappingParameterAt(a->param, index);
			index++;
		}
	}
}


var CustomOSCCommand::getJSONData()
{
	var data = OSCCommand::getJSONData();
	data.getDynamicObject()->setProperty("argManager", argManager.getJSONData());
	return data;
}

void CustomOSCCommand::loadJSONDataInternal(var data)
{
	OSCCommand::loadJSONDataInternal(data);
	argManager.loadJSONData(data.getProperty("argManager", var()), true);
}