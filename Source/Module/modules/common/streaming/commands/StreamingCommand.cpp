/*
  ==============================================================================

	StreamingCommand.cpp
	Created: 27 Jan 2018 12:22:20pm
	Author:  Ben

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

StreamingCommand::StreamingCommand(StreamingModule* _module, CommandContext context, var params, Multiplex* multiplex) :
	BaseCommand(_module, context, params, multiplex),
	streamingModule(_module)
{
	if (params.hasProperty("customParams"))
	{
		var cParams = params.getProperty("customParams", var());
		for (int i = 0; i < cParams.size(); i++)
		{
			DynamicObject* cp = cParams[i].getDynamicObject();
			if (cp == nullptr) continue;
			if (!cp->hasProperty("name")) continue;
			Parameter* p = dynamic_cast<Parameter*>(ControllableFactory::createControllableFromJSON(cp->getProperty("name"), cp));
			if (p == nullptr) return;

			addParameter(p, cParams[i].getProperty("index", -1));
			customParams.add(p);
		}
	}
}

StreamingCommand::~StreamingCommand()
{

}

var StreamingCommand::getCustomParams(int multiplexIndex)
{
	var cData(new DynamicObject());
	for (auto& p : customParams)
	{
		if (!p->enabled) continue;
		cData.getDynamicObject()->setProperty(p->shortName, getLinkedValue(p, multiplexIndex));
	}

	return cData;
}
