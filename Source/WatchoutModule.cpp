/*
  ==============================================================================

    WatchoutModule.cpp
    Created: 2 Jan 2018 11:54:57am
    Author:  Ben

  ==============================================================================
*/

#include "WatchoutModule.h"
#include "WatchoutCommand.h"

WatchoutModule::WatchoutModule() :
	TCPModule("Watchout",3040)
{
	defManager.add(CommandDefinition::createDef(this, "", "Ping", &WatchoutCommand::create, CommandContext::ACTION)->addParam("fixedValue", "ping")->addParam("forceCR", true)->addParam("forceNL", true));
}

WatchoutModule::~WatchoutModule()
{
}
