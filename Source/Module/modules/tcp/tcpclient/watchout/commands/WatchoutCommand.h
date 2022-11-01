/*
  ==============================================================================

    WatchoutCommand.h
    Created: 2 Jan 2018 2:02:14pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

class WatchoutCommand :
	public SendStreamStringCommand
{
public:
	WatchoutCommand(WatchoutModule * _module, CommandContext context, var params, Multiplex * multiplex = nullptr);
	~WatchoutCommand();

	WatchoutModule * wModule;

	Array<Parameter*> argsParams;

	void triggerInternal(int multiplexIndex) override;

	static WatchoutCommand * create(ControllableContainer * module, CommandContext context, var params, Multiplex * multiplex) { return new WatchoutCommand((WatchoutModule *)module, context, params, multiplex); }

};
