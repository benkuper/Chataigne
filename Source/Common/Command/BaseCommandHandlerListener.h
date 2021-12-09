/*
  ==============================================================================

    BaseCommandHandlerListener.h
    Created: 9 Dec 2021 9:36:44am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
class BaseCommandHandler;

class CommandHandlerListener
{
public:
	virtual ~CommandHandlerListener() {}
	virtual void commandChanged(BaseCommandHandler*) {}
	virtual void commandUpdated(BaseCommandHandler*) {}
};

class CommandHandlerEvent {
public:
	enum Type { COMMAND_CHANGED, COMMAND_UPDATED };
	CommandHandlerEvent(Type type, BaseCommandHandler* h) : type(type), handler(h) {}
	Type type;
	BaseCommandHandler* handler;
};

typedef QueuedNotifier<CommandHandlerEvent>::Listener BaseCommandHandlerAsyncListener;

