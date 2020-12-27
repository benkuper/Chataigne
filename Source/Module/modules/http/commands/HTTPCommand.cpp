/*
  ==============================================================================

    HTTPCommand.cpp
    Created: 13 Feb 2019 11:28:44am
    Author:  bkupe

  ==============================================================================
*/

#include "HTTPCommand.h"

HTTPCommand::HTTPCommand(HTTPModule * _module, CommandContext context, var params, IteratorProcessor* iterator) :
	BaseCommand(_module, context, params, iterator),
	httpModule(_module)
{
	method = addEnumParameter("Method", "Request Method");
	for (int i = 0; i < HTTPModule::TYPE_MAX; ++i) method->addOption(HTTPModule::requestMethodNames[i], (HTTPModule::RequestMethod)i);

	resultDataType = addEnumParameter("Result Type", "The type of data to parse the received data. Default will let the module decide, depending on its Protocol parameter");
	resultDataType->addOption("Default", HTTPModule::DEFAULT)->addOption("Raw", HTTPModule::RAW)->addOption("JSON", HTTPModule::JSON)->addOption("XML", HTTPModule::XML);

	address = addStringParameter("Address", "Address to append to the module's base address", "anything");

	setUseCustomValues(true);

	extraHeaders = addStringParameter("Extra Headers", "HTTP headers to add to the request", "");
	extraHeaders->multiline = true;
}

HTTPCommand::~HTTPCommand()
{
}


void HTTPCommand::triggerInternal(int iterationIndex)
{
	StringPairArray requestParams;
	for (auto &p : customValuesManager->items) requestParams.set(p->niceName, p->getLinkedValue(iterationIndex));
	
	StringPairArray headers;
	
	httpModule->sendRequest(getLinkedValue(address, iterationIndex).toString(), method->getValueDataAsEnum<HTTPModule::RequestMethod>(), resultDataType->getValueDataAsEnum<HTTPModule::ResultDataType>(), requestParams, getLinkedValue(extraHeaders, iterationIndex).toString());
}