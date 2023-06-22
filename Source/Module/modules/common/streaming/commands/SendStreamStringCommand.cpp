/*
  ==============================================================================

	SendStreamStringCommand.cpp
	Created: 21 Oct 2017 5:05:39pm
	Author:  Ben

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

SendStreamStringCommand::SendStreamStringCommand(StreamingModule* _module, CommandContext context, var params, Multiplex* multiplex) :
	StreamingCommand(_module, context, params, multiplex),
	appendCR(nullptr),
	appendNL(nullptr),
	prefix(nullptr)
{
	if (context == CommandContext::MAPPING)
	{
		prefix = addStringParameter("Prefix", "Add some data before the mapped value\nThis does NOT automatically append a whitespace !", "");
	}

	valueParam = addStringParameter("Value", "Value to send via TCP", "example");
	valueParam->multiline = true;

	dataMode = (DataMode)(int)params.getProperty("mode", DataMode::STRING);
	switch (dataMode)
	{
	case STRING:

		if (params.hasProperty("fixedValue"))
		{
			valueParam->setControllableFeedbackOnly(true);
			valueParam->setValue(params.getProperty("fixedValue", ""));
			valueParam->multiline = false;
		}

		if (!params.getProperty("hideNLCR", false))
		{
			appendCR = addBoolParameter("Append CR", "Append \\r at the end of the message", false);
			appendNL = addBoolParameter("Append NL", "Append \\n at the end of the message", true);

			if (params.hasProperty("forceCR"))
			{
				appendCR->setValue(params.getProperty("forceCR", false));
				appendCR->hideInEditor = true;
			}

			if (params.hasProperty("forceNL"))
			{
				appendNL->setValue(params.getProperty("forceNL", true));
				appendNL->hideInEditor = true;
			}
		}

		if (context == CommandContext::MAPPING)
		{
			linkParamToMappingIndex(valueParam, 0);
		}


		break;

	case HEX:
		valueParam->defaultUI = StringParameter::TEXT;
		break;
	}


}

SendStreamStringCommand::~SendStreamStringCommand()
{
}

void SendStreamStringCommand::setValue(var value, int multiplexIndex)
{
	switch (dataMode)
	{
	case STRING:
		break;

	case HEX:
		mappedValues.resize(value.isArray() ? value.size() : 1);
		if (value.isArray())
		{
			for (int i = 0; i < value.size(); ++i) mappedValues.set(i, (int)value[i]);
		}
		else
		{
			mappedValues.set(0, (int)value);
		}
		break;
	}

	StreamingCommand::setValue(value, multiplexIndex);
}

void SendStreamStringCommand::triggerInternal(int multiplexIndex)
{
	String valString = getLinkedValue(valueParam, multiplexIndex).toString();

	switch (dataMode)
	{
	case STRING:
	{
		if (prefix != nullptr) valString = getLinkedValue(prefix, multiplexIndex).toString() + valString;
		if (appendCR != nullptr && (bool)getLinkedValue(appendCR, multiplexIndex)) valString += "\r";
		if (appendNL != nullptr && (bool)getLinkedValue(appendNL, multiplexIndex)) valString += "\n";

		streamingModule->sendMessage(valString, getCustomParams(multiplexIndex));
	}
	break;

	case HEX:
	{
		StringArray hexValues;
		hexValues.addTokens(valString, " ");
		Array<uint8> values;

		if (hexValues.size() == 1 && valString.length() % 2 == 0) //no spaces, even number of characters, separate 2 by 2
		{
			for (int i = 0; i < valString.length(); i += 2) values.add(valString.substring(i, i + 2).getHexValue32());
			streamingModule->sendBytes(values, getCustomParams(multiplexIndex));

		}
		else
		{
			for (auto& v : hexValues)
			{
				if (v.startsWithChar('%'))
				{
					int index = v.substring(1).getIntValue() - 1;
					if (index >= 0 && index < mappedValues.size()) values.add(mappedValues[index]);
					else values.add(0);
				}
				else
				{
					values.add(v.getHexValue32());
				}
			}

			streamingModule->sendBytes(values, getCustomParams(multiplexIndex));
		}
	}
	break;

	}

}
