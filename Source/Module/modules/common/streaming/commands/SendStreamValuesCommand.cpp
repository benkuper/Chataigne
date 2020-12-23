/*
  ==============================================================================

    SendStreamValuesCommand.cpp
    Created: 27 Jan 2018 12:04:01pm
    Author:  Ben

  ==============================================================================
*/

#include "SendStreamValuesCommand.h"

SendStreamValuesCommand::SendStreamValuesCommand(StreamingModule * module, CommandContext context, var params, IteratorProcessor* iterator) :
	StreamingCommand(module, context, params, iterator)
{
	customValuesManager.reset(new CustomValuesCommandArgumentManager(context == MAPPING));
	addChildControllableContainer(customValuesManager.get());
	setUseCustomValues(true);
}

SendStreamValuesCommand::~SendStreamValuesCommand()
{
	masterReference.clear();
}

var SendStreamValuesCommand::getJSONData()
{
	var data = StreamingCommand::getJSONData();
	data.getDynamicObject()->setProperty("argManager", customValuesManager->getJSONData());
	return data;
}

void SendStreamValuesCommand::loadJSONDataInternal(var data)
{
	StreamingCommand::loadJSONDataInternal(data);
	customValuesManager->loadJSONData(data.getProperty("argManager", var()), true);
}

void SendStreamValuesCommand::triggerInternal(int iterationIndex)
{
	if (streamingModule == nullptr) return;
	
	MemoryOutputStream data;

	for (auto &a : customValuesManager->items)
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
			data.writeFloat(((Point2DParameter *)p)->x);
			data.writeFloat(((Point2DParameter *)p)->y);
			break;
		case Controllable::POINT3D:
			data.writeFloat(((Point3DParameter *)p)->x);
			data.writeFloat(((Point3DParameter *)p)->y);
			data.writeFloat(((Point3DParameter *)p)->z);
			break;

		case Controllable::COLOR:
		{
			Colour c = ((ColorParameter*)p)->getColor();
			data.writeFloat(c.getFloatRed());
			data.writeFloat(c.getFloatGreen());
			data.writeFloat(c.getFloatBlue());
			data.writeFloat(c.getFloatAlpha());
		}
			break;

		default:
			//not handle
			break;

		}
	}

	Array<uint8> bytes((uint8 *)data.getData(), (int)data.getDataSize());
	streamingModule->sendBytes(bytes);
}