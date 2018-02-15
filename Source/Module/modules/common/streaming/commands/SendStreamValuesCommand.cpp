/*
  ==============================================================================

    SendStreamValuesCommand.cpp
    Created: 27 Jan 2018 12:04:01pm
    Author:  Ben

  ==============================================================================
*/

#include "SendStreamValuesCommand.h"

SendStreamValuesCommand::SendStreamValuesCommand(StreamingModule * module, CommandContext context, var params) :
	StreamingCommand(module, context, params),
	argManager(context == MAPPING)
{
	addChildControllableContainer(&argManager);
	argManager.addArgumentManagerListener(this);
}

SendStreamValuesCommand::~SendStreamValuesCommand()
{
	masterReference.clear();
}

var SendStreamValuesCommand::getJSONData()
{
	var data = StreamingCommand::getJSONData();
	data.getDynamicObject()->setProperty("argManager", argManager.getJSONData());
	return data;
}

void SendStreamValuesCommand::loadJSONDataInternal(var data)
{
	StreamingCommand::loadJSONDataInternal(data);
	argManager.loadJSONData(data.getProperty("argManager", var()), true);
}

void SendStreamValuesCommand::trigger()
{
	if (streamingModule == nullptr) return;
	
	MemoryOutputStream data;

	for (auto &a : argManager.items)
	{
		Parameter * p = a->param;
		if (p == nullptr) continue;
		switch (p->type)
		{
		case Controllable::BOOL: data.writeBool(p->boolValue());
		case Controllable::INT: data.writeInt(p->intValue()); break;
		case Controllable::FLOAT: data.writeFloat(p->floatValue()); break;
		case Controllable::STRING: data.writeString(p->stringValue()); break;
		case Controllable::POINT2D:
			data.writeFloat(((Point2DParameter *)a)->x);
			data.writeFloat(((Point2DParameter *)a)->y);
			break;
		case Controllable::POINT3D:
			data.writeFloat(((Point3DParameter *)a)->x);
			data.writeFloat(((Point3DParameter *)a)->y);
			data.writeFloat(((Point3DParameter *)a)->z);
			break;

		default:
			//not handle
			break;

		}
	}

	Array<uint8> bytes((uint8 *)data.getData(), (int)data.getDataSize());
	streamingModule->sendBytes(bytes);
}

void SendStreamValuesCommand::useForMappingChanged(CustomValuesCommandArgument *)
{
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