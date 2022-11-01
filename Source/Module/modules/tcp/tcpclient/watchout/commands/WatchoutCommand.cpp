/*
  ==============================================================================

	WatchoutCommand.cpp
	Created: 2 Jan 2018 2:02:14pm
	Author:  Ben

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

WatchoutCommand::WatchoutCommand(WatchoutModule* _module, CommandContext context, var params, Multiplex* multiplex) :
	SendStreamStringCommand(_module, context, params),
	wModule(_module)
{
	autoLoadPreviousCommandData = true;

	String argsP = params.getProperty("args", "").toString();
	//if (argsP.isNotEmpty())
	//{
	//	paramContainer.reset(new ControllableContainer("Parameters"));
	//	addChildControllableContainer(paramContainer.get());
	//}

	StringArray argsSplit;
	argsSplit.addTokens(argsP, true);

	for (auto& s : argsSplit.strings)
	{
		StringArray sp;
		sp.addTokens(s, ":", "\"");
		if (sp.size() >= 2)
		{
			Parameter* p = nullptr;
			if (sp[0] == "b") p = addBoolParameter(sp[1], sp[1], false);
			else if (sp[0] == "f") p = addFloatParameter(sp[1], sp[1], 0, 0, 1);
			else if (sp[0] == "i") p = addIntParameter(sp[1], sp[1], 0, 0, 1000000);
			else if (sp[0] == "s") p = addStringParameter(sp[1], sp[1], "");

			argsParams.add(p);
		}
	}
}

WatchoutCommand::~WatchoutCommand()
{
}

void WatchoutCommand::triggerInternal(int multiplexIndex)
{
	BaseCommand::triggerInternal(multiplexIndex);

	String s = getLinkedValue(valueParam, multiplexIndex);

	for (auto& p : argsParams)
	{
		s += " ";
		switch (p->type)
		{
		case Controllable::BOOL:
			s += p->boolValue() ? "true" : "false";
			break;

		case Controllable::STRING:
			s += "\"" + p->stringValue() + "\"";
			break;

		case Controllable::INT:
			s += String(p->intValue());
			break;

		case Controllable::FLOAT:
			s += String(p->floatValue());
			break;

		default:
			//NOT HANDLED
			break;
		}
	}

	s += "\r\n";

	wModule->sendMessage(s);
}
