/*
  ==============================================================================

    ChataigneGenericModule.cpp
    Created: 28 Apr 2017 5:50:41pm
    Author:  Ben

  ==============================================================================
*/

#include "ChataigneGenericModule.h"
#include "commands/GenericControllableCommand.h"
#include "commands/ChataigneLogCommand.h"
#include "commands/GenericScriptCommand.h"
#include "commands/GenericAppCommand.h"

ChataigneGenericModule::ChataigneGenericModule() :
	Module("Generic")
{
	defManager.add(CommandDefinition::createDef(this, "", "Set Parameter Value", &GenericControllableCommand::create)->addParam("action", GenericControllableCommand::SET_VALUE));
	defManager.add(CommandDefinition::createDef(this, "", "Trigger a Control", &GenericControllableCommand::create)->addParam("action", GenericControllableCommand::TRIGGER));
	defManager.add(CommandDefinition::createDef(this, "", "Log a message", &ChataigneLogCommand::create, CommandContext::ACTION)->addParam("type", ChataigneLogCommand::MESSAGE));
	defManager.add(CommandDefinition::createDef(this, "", "Log a value", &ChataigneLogCommand::create)->addParam("type", ChataigneLogCommand::VALUE));
	defManager.add(CommandDefinition::createDef(this, "", "New Session", &GenericAppCommand::create, CommandContext::ACTION)->addParam("type", GenericAppCommand::NEW_SESSION));
	defManager.add(CommandDefinition::createDef(this, "", "Open Session", &GenericAppCommand::create, CommandContext::ACTION)->addParam("type", GenericAppCommand::OPEN_SESSION));
	defManager.add(CommandDefinition::createDef(this, "", "Close App", &GenericAppCommand::create, CommandContext::ACTION)->addParam("type", GenericAppCommand::CLOSE_APP));
	defManager.add(CommandDefinition::createDef(this, "", "Script", &GenericScriptCommand::create));
}

ChataigneGenericModule::~ChataigneGenericModule()
{
}
