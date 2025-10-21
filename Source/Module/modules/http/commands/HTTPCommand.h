/*
  ==============================================================================

    HTTPCommand.h
    Created: 13 Feb 2019 11:28:44am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class HTTPCommand :
	public BaseCommand
{
public:
	HTTPCommand(HTTPModule * _module, CommandContext context, var params, Multiplex* multiplex = nullptr);
	~HTTPCommand();

	enum ContentType { URLENCODED, PLAIN, FILE };

	ContentType contentType;

	HTTPModule * httpModule;

	StringParameter * address;
	EnumParameter * method;
	EnumParameter * resultDataType;

	StringParameter* extraHeaders;
	StringParameter* payload;
	FileParameter* file;

	void triggerInternal(int multiplexIndex) override;

	var getJSONData(bool includeNonOverriden = false) override;
	void loadJSONDataInternal(var data) override;

	static BaseCommand * create(ControllableContainer * module, CommandContext context, var params, Multiplex * multiplex) { return new HTTPCommand((HTTPModule *)module, context, params, multiplex); }
};
