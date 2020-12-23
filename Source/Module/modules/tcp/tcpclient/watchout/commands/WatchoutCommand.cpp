/*
  ==============================================================================

    WatchoutCommand.cpp
    Created: 2 Jan 2018 2:02:14pm
    Author:  Ben

  ==============================================================================
*/

#include "WatchoutCommand.h"

WatchoutCommand::WatchoutCommand(WatchoutModule * _module, CommandContext context, var params, IteratorProcessor * iterator) :
	SendStreamStringCommand(_module,context,params),
	wModule(_module)
{
	String argsP = params.getProperty("args", "").toString();
	if (argsP.isNotEmpty())
	{
		paramContainer.reset(new ControllableContainer("Parameters"));
		addChildControllableContainer(paramContainer.get());
	}

	StringArray argsSplit;
	argsSplit.addTokens(argsP, true);
	for (auto &s : argsSplit.strings)
	{
		StringArray sp;
		sp.addTokens(s, ":", "\"");
		if (sp.size() >= 2)
		{
			if (sp[0] == "b") paramContainer->addBoolParameter(sp[1], sp[1], false);
			else if (sp[0] == "f") paramContainer->addFloatParameter(sp[1], sp[1], 0, 0, 1);
			else if (sp[0] == "i") paramContainer->addIntParameter(sp[1], sp[1], 0, 0, 1000000);
			else if (sp[0] == "s") paramContainer->addStringParameter(sp[1], sp[1], "");
		}
	}
}

WatchoutCommand::~WatchoutCommand()
{
}

void WatchoutCommand::triggerInternal()
{
	BaseCommand::triggerInternal();

	String s = valueParam->stringValue();
	if (paramContainer != nullptr)
	{
		Array<WeakReference<Parameter>> pList = paramContainer->getAllParameters();
		for (auto &p : pList)
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
	}

	s += "\r\n";

	wModule->sendMessage(s);
}
