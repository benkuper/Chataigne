/*
  ==============================================================================

    SendStreamStringValuesCommand.cpp
    Created: 30 May 2018 12:01:59pm
    Author:  Ben

  ==============================================================================
*/

#include "SendStreamStringValuesCommand.h"

SendStreamStringValuesCommand::SendStreamStringValuesCommand(StreamingModule * output, CommandContext context, var params, Multiplex* multiplex) :
	SendStreamValuesCommand(output,context,params, multiplex)
{
	prefix = addStringParameter("Prefix", "This will be prepended to the final string", "");
	separator = addStringParameter("Separator", "The string that separate each values", ",");
	suffix = addStringParameter("Suffix","This will be appended to the final string, but before NL and CR if selected","");

	appendCR = addBoolParameter("Append CR", "Append \\r at the end of the message", false);
	if (params.hasProperty("forceCR"))
	{
		appendCR->setValue(params.getProperty("forceCR", false));
		appendCR->hideInEditor = true;
	}

	appendNL = addBoolParameter("Append NL", "Append \\n at the end of the message", true);
	if (params.hasProperty("forceNL"))
	{
		appendNL->setValue(params.getProperty("forceNL", true));
		appendNL->hideInEditor = true;
	}
}

SendStreamStringValuesCommand::~SendStreamStringValuesCommand()
{
}

void SendStreamStringValuesCommand::triggerInternal(int multiplexIndex)
{
	if (streamingModule == nullptr) return;
	String s = "";
	String sepStr = getLinkedValue(separator, multiplexIndex);

	for (auto &a : customValuesManager->items)
	{
		if (s.length() > 0) s += sepStr;
		
		var val = a->getLinkedValue(multiplexIndex);
		if (val.isVoid()) continue;
		String ss = "";

		if (val.isArray())
		{
			for (int i = 0; i < val.size(); ++i)
			{
				if (i > 0) ss += sepStr;
				ss += String((float)val[i], 0);
			}
		} else
		{
			ss += val.toString();
		}

		s += ss;
	}

	s = prefix->stringValue() + s + getLinkedValue(suffix, multiplexIndex).toString();
	if (appendCR->boolValue()) s += "\r";
	if (appendNL->boolValue()) s += "\n";

	streamingModule->sendMessage(s);
}
