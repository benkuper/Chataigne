/*
  ==============================================================================

    SendStreamStringValuesCommand.cpp
    Created: 30 May 2018 12:01:59pm
    Author:  Ben

  ==============================================================================
*/

#include "SendStreamStringValuesCommand.h"

SendStreamStringValuesCommand::SendStreamStringValuesCommand(StreamingModule * output, CommandContext context, var params) :
	SendStreamValuesCommand(output,context,params)
{
	separator = addStringParameter("Separator","The string that separate each values"," ");

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

void SendStreamStringValuesCommand::triggerInternal()
{
	if (streamingModule == nullptr) return;
	String s = "";
	for (auto &a : customValuesManager->items)
	{
		if (s.length() > 0) s += separator->stringValue();
		Parameter * p = a->param;
		if (p == nullptr) continue;
		String ss = "";

		if (p->value.isArray())
		{
			
			for (int i = 0; i < p->value.size(); i++)
			{
				if (i > 0) ss += separator->stringValue();
				ss += String((float)p->value[i], 0);
			}
		} else
		{
			ss += p->stringValue();
		}

		s += ss;
	}

	if (appendCR->boolValue()) s += "\r";
	if (appendNL->boolValue()) s += "\n";
	streamingModule->sendMessage(s);
}
