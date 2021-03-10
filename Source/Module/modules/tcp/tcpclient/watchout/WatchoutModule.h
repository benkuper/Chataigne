/*
  ==============================================================================

    WatchoutModule.h
    Created: 2 Jan 2018 11:54:57am
    Author:  Ben

  ==============================================================================
*/

#pragma once

class WatchoutModule :
	public TCPClientModule
{
public:
	WatchoutModule();
	~WatchoutModule();

	CommandDefinition * getBaseWCommand(const String &menu, const String &commandName, const String &command, const String &params = "", CommandContext context = CommandContext::ACTION);

	static WatchoutModule * create() { return new WatchoutModule(); }
	virtual String getDefaultTypeString() const override { return "Watchout"; }
};