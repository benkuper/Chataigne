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
	public BaseCommand
{
public:
	HTTPCommand(HTTPModule * _module, CommandContext context, var params, IteratorProcessor* iterator = nullptr);
	~HTTPCommand();

	HTTPModule * httpModule;
	

	StringParameter * address;
	EnumParameter * method;
	EnumParameter * resultDataType;

	StringParameter* extraHeaders;

	void triggerInternal() override;

	static BaseCommand * create(ControllableContainer * module, CommandContext context, var params, IteratorProcessor * iterator) { return new HTTPCommand((HTTPModule *)module, context, params, iterator); }
};
