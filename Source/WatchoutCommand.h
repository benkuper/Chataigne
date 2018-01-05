/*
  ==============================================================================

    WatchoutCommand.h
    Created: 2 Jan 2018 2:02:14pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "SendStreamStringCommand.h"
#include "WatchoutModule.h"

class WatchoutCommand :
	public SendStreamStringCommand
{
public:
	WatchoutCommand(WatchoutModule * _module, CommandContext context, var params);
	~WatchoutCommand();

	WatchoutModule * wModule;

	ScopedPointer<ControllableContainer> paramContainer;

	void trigger() override;

	static WatchoutCommand * create(ControllableContainer * module, CommandContext context, var params) { return new WatchoutCommand((WatchoutModule *)module, context, params); }

};