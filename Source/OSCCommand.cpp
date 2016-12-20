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
	ModuleCommand(_module,context, params),
	oscModule(_module),
	argumentsContainer("Arguments")
{
	address = addStringParameter("Address", "Adress of the OSC Message (e.g. /example)", params.getProperty("address","/example"));
	addChildControllableContainer(&argumentsContainer);
}

OSCCommand::~OSCCommand()
{
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
