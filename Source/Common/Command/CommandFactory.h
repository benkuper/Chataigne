/*
  ==============================================================================

    CommandFactory.h
    Created: 3 Nov 2016 12:24:41pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "CommandDefinition.h"

class Module;

class CommandFactory
{
public:
    static CommandDefinition* showMenuAndGetCommand(CommandContext context, Module* lockedModule = nullptr, bool multiplexMode = false);
    static PopupMenu getCommandMenu(CommandContext context, Module* lockedModule = nullptr, bool multiplexMode = false);
    static CommandDefinition* getCommandFromResult(int result, Module * lockedModule);
};
