/*
  ==============================================================================

    WatchoutCommand.h
    Created: 2 Jan 2018 2:02:14pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../../../../common/streaming/commands/SendStreamStringCommand.h"
#include "../WatchoutModule.h"

class WatchoutCommand :
	public SendStreamStringCommand
{
public:
	WatchoutCommand(WatchoutModule * _module, CommandContext context, var params, IteratorProcessor * iterator = nullptr);
	~WatchoutCommand();

	WatchoutModule * wModule;

	std::unique_ptr<ControllableContainer> paramContainer;

	void triggerInternal(int iterationIndex) override;

	static WatchoutCommand * create(ControllableContainer * module, CommandContext context, var params, IteratorProcessor * iterator) { return new WatchoutCommand((WatchoutModule *)module, context, params, iterator); }

};
