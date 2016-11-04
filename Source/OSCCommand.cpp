/*
  ==============================================================================

    OSCCommand.cpp
    Created: 3 Nov 2016 10:47:46am
    Author:  bkupe

  ==============================================================================
*/

#include "OSCCommand.h"
#include "OSCOutput.h"

OSCCommand::OSCCommand(OSCOutput * _output, CommandContext context, var params) :
	OutputCommand(_output,context, params),
	oscOutput(_output)
{
	address = addStringParameter("Address", "Adress of the OSC Message (e.g. /example)", params.getProperty("address","/example"));
}

OSCCommand::~OSCCommand()
{
}

void OSCCommand::trigger()
{
	if (oscOutput == nullptr) return;

	OSCMessage m(address->stringValue());
	
	for (auto &a : arguments)
	{
		switch (a->type)
		{
		case Controllable::BOOL: m.addInt32(a->boolValue() ? 1 : 0); break;
		case Controllable::INT: m.addInt32(a->intValue()); break;
		case Controllable::FLOAT: m.addFloat32(a->floatValue()); break;
		case Controllable::STRING: m.addString(a->stringValue()); break;
		case Controllable::POINT2D:
			m.addFloat32(((Point2DParameter *)a)->x);
			m.addFloat32(((Point2DParameter *)a)->y);
			break;
		case Controllable::POINT3D:
			m.addFloat32(((Point3DParameter *)a)->x);
			m.addFloat32(((Point3DParameter *)a)->y);
			m.addFloat32(((Point3DParameter *)a)->z);
			break;

		}
	}

	oscOutput->sendOSC(m);
}
