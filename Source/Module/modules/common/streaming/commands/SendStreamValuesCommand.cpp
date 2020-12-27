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
		
		var val = a->getLinkedValue(iterationIndex);

		switch (p->type)
		{
		case Controllable::BOOL: data.writeBool(val);
		case Controllable::INT: data.writeInt(val); break;
		case Controllable::FLOAT: data.writeFloat(val); break;
		case Controllable::STRING: data.writeString(val); break;

		default:
			if (val.isArray()) for (int i = 0; i < val.size(); i++) data.writeFloat(val[i]);
			break;
		}
	}

	Array<uint8> bytes((uint8 *)data.getData(), (int)data.getDataSize());
	streamingModule->sendBytes(bytes);
}