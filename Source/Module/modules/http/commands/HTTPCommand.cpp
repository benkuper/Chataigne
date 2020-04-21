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
	for (int i = 0; i < HTTPModule::TYPE_MAX; i++) method->addOption(HTTPModule::requestMethodNames[i], (HTTPModule::RequestMethod)i);

	resultDataType = addEnumParameter("Result Type", "The type of data to parse the received data");
	resultDataType->addOption("Raw", HTTPModule::RAW)->addOption("JSON", HTTPModule::JSON);

	address = addStringParameter("Address", "Address to append to the module's base address", "anything");

	setUseCustomValues(true);

	extraHeaders = addStringParameter("Extra Headers", "HTTP headers to add to the request", "");
	extraHeaders->multiline = true;
}

HTTPCommand::~HTTPCommand()
{
}


void HTTPCommand::triggerInternal()
{
	StringPairArray requestParams;
	for (auto &p : customValuesManager->items) requestParams.set(p->niceName, p->param->stringValue());
	
	StringPairArray headers;
	
	httpModule->sendRequest(address->stringValue(), method->getValueDataAsEnum<HTTPModule::RequestMethod>(), resultDataType->getValueDataAsEnum<HTTPModule::ResultDataType>(), requestParams, extraHeaders->stringValue());
}