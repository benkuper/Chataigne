/*
  ==============================================================================

    WatchoutModule.h
    Created: 2 Jan 2018 11:54:57am
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../TCPModule.h"

class WatchoutModule :
	public TCPModule
{
public:
	WatchoutModule();
	~WatchoutModule();

	CommandDefinition * getBaseWCommand(const String &menu, const String &commandName, const String &command, const String &params = "", CommandContext context = CommandContext::ACTION);

	static WatchoutModule * create() { return new WatchoutModule(); }
	virtual String getDefaultTypeString() const override { return "Watchout"; }
};