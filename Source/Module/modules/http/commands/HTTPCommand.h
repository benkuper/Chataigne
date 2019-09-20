/*
  ==============================================================================

    HTTPCommand.h
    Created: 13 Feb 2019 11:28:44am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../HTTPModule.h"

class HTTPCommand :
	public BaseCommand,
	public CustomValuesCommandArgumentManager::ArgumentManagerListener
{
public:
	HTTPCommand(HTTPModule * _module, CommandContext context, var params);
	~HTTPCommand();

	HTTPModule * httpModule;
	

	StringParameter * address;
	EnumParameter * method;
	EnumParameter * resultDataType;

	StringParameter* extraHeaders;

	void triggerInternal() override;

	void useForMappingChanged(CustomValuesCommandArgument *) override;

	static BaseCommand * create(ControllableContainer * module, CommandContext context, var params) { return new HTTPCommand((HTTPModule *)module, context, params); }
};
