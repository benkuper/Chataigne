/*
  ==============================================================================

    HTTPCommand.cpp
    Created: 13 Feb 2019 11:28:44am
    Author:  bkupe

  ==============================================================================
*/

#include "HTTPCommand.h"

HTTPCommand::HTTPCommand(HTTPModule * _module, CommandContext context, var params) :
	BaseCommand(_module, context, params),
	httpModule(_module)
{
	method = addEnumParameter("Method", "Request Method");
	method->addOption("GET", HTTPModule::GET)->addOption("POST", HTTPModule::POST);

	address = addStringParameter("Address", "Address to append to the module's base address", "anything");

	customValuesManager = new CustomValuesCommandArgumentManager(context == MAPPING);
	addChildControllableContainer(customValuesManager);
	customValuesManager->addArgumentManagerListener(this);
}

HTTPCommand::~HTTPCommand()
{
}


void HTTPCommand::triggerInternal()
{
	StringPairArray requestParams;
	for (auto &p : customValuesManager->items) requestParams.set(p->shortName, p->param->stringValue());
	httpModule->sendRequest(address->stringValue(), method->getValueDataAsEnum<HTTPModule::RequestMethod>(), requestParams);
}

void HTTPCommand::useForMappingChanged(CustomValuesCommandArgument *)
{
	if (context != CommandContext::MAPPING) return;

	clearTargetMappingParameters();
	int index = 0;
	for (auto &a : customValuesManager->items)
	{
		if (a->useForMapping != nullptr && a->useForMapping->boolValue())
		{
			addTargetMappingParameterAt(a->param, index);
			index++;
		}
	}
}
