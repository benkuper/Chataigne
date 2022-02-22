/*
  ==============================================================================

    CommandFactory.h
    Created: 3 Nov 2016 12:24:41pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class Module;

class CommandFactory
{
public:
    static void showMenuAndGetCommand(CommandContext context, std::function<void(CommandDefinition*)> returnFunc, Module* lockedModule = nullptr, bool multiplexMode = false);
    static PopupMenu getCommandMenu(CommandContext context, Module* lockedModule = nullptr, bool multiplexMode = false);
    static CommandDefinition* getCommandFromResult(int result, Module * lockedModule);
};
