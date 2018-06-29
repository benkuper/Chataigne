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

ChataigneGenericModule::ChataigneGenericModule() :
	Module("Generic")
{
	defManager.add(CommandDefinition::createDef(this, "", "Set Parameter Value", &GenericControllableCommand::create)->addParam("action", GenericControllableCommand::SET_VALUE));
	defManager.add(CommandDefinition::createDef(this, "", "Trigger a Control", &GenericControllableCommand::create)->addParam("action", GenericControllableCommand::TRIGGER));
	defManager.add(CommandDefinition::createDef(this, "", "Log a message", &ChataigneLogCommand::create, CommandContext::ACTION)->addParam("type", ChataigneLogCommand::MESSAGE));
	defManager.add(CommandDefinition::createDef(this, "", "Log a value", &ChataigneLogCommand::create)->addParam("type",ChataigneLogCommand::VALUE));
}

ChataigneGenericModule::~ChataigneGenericModule()
{
}
