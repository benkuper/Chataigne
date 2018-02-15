/*
  ==============================================================================

    PJLinkModule.cpp
    Created: 5 Jan 2018 3:41:58pm
    Author:  Ben

  ==============================================================================
*/

#include "PJLinkModule.h"
#include "../../common/streaming/commands/SendStreamStringCommand.h"

PJLinkModule::PJLinkModule() :
	TCPModule(getDefaultTypeString(), 4352)
{
	defManager.add(getBasePJCommand("", "Power On", "%1POWR 1", CommandContext::ACTION));
	defManager.add(getBasePJCommand("", "Power Off", "%1POWR 0", CommandContext::ACTION));
	defManager.add(getBasePJCommand("", "Shutter On", "%1AVMT 1", CommandContext::ACTION));
	defManager.add(getBasePJCommand("", "Shutter Off", "%1AVMT 0", CommandContext::ACTION));
}

CommandDefinition * PJLinkModule::getBasePJCommand(const String & menu, const String & commandName, const String & command, CommandContext context)
{
	return CommandDefinition::createDef(this, menu, commandName, &SendStreamStringCommand::create, context)->addParam("fixedValue", command)->addParam("forceCR", true)->addParam("forceNL", false);

}