/*
  ==============================================================================

    WatchoutCommand.cpp
    Created: 2 Jan 2018 2:02:14pm
    Author:  Ben

  ==============================================================================
*/

#include "WatchoutCommand.h"

WatchoutCommand::WatchoutCommand(WatchoutModule * _module, CommandContext context, var params) :
	SendTCPStringCommand(_module,context,params)
{
	String argsP = params.getProperty("args", "").toString();
	if (argsP.isNotEmpty())
	{
		paramContainer = new ControllableContainer("Parameters");
		addChildControllableContainer(paramContainer);
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
			else if (sp[0] == "i") paramContainer->addIntParameter(sp[1], sp[1], 0, 0, 100000);
			else if (sp[0] == "s") paramContainer->addStringParameter(sp[1], sp[1], "");
		}
	}
}

WatchoutCommand::~WatchoutCommand()
{
}

void WatchoutCommand::trigger()
{
}
