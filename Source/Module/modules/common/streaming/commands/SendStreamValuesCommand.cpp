/*
  ==============================================================================

    SendStreamValuesCommand.cpp
    Created: 27 Jan 2018 12:04:01pm
    Author:  Ben

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

SendStreamValuesCommand::SendStreamValuesCommand(StreamingModule * module, CommandContext context, var params, Multiplex* multiplex) :
	StreamingCommand(module, context, params, multiplex)
{
	setUseCustomValues(true);
}

SendStreamValuesCommand::~SendStreamValuesCommand()
{
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

void SendStreamValuesCommand::triggerInternal(int multiplexIndex)
{
	if (streamingModule == nullptr) return;
	
	MemoryOutputStream data;

	for (auto &a : customValuesManager->items)
	{
		Parameter * p = a->param;
		if (p == nullptr) continue;
		
		var val = a->getLinkedValue(multiplexIndex);

		switch (p->type)
		{
		case Controllable::BOOL: data.writeBool(val);
		case Controllable::INT:
			switch (a->sendPrecision->getValueDataAsEnum<CustomValuesCommandArgument::IntType>())
			{
				case CustomValuesCommandArgument::INT16:
					data.writeShort((int)val);
					break;
				case CustomValuesCommandArgument::BYTE:
					data.writeByte((int)val);
					break;
				default:
					data.writeInt(val);
					break;
			}
			break;
		case Controllable::FLOAT: data.writeFloat(val); break;
		case Controllable::STRING: data.writeString(val); break;

		default:
			if (val.isArray()) for (int i = 0; i < val.size(); i++) data.writeFloat(val[i]);
			break;
		}
	}

	Array<uint8> bytes((uint8 *)data.getData(), (int)data.getDataSize());
	streamingModule->sendBytes(bytes, getCustomParams(multiplexIndex));
}