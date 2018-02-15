/*
  ==============================================================================

    ChataigneGenericModule.cpp
    Created: 28 Apr 2017 5:50:41pm
    Author:  Ben

  ==============================================================================
*/

#include "ChataigneGenericModule.h"
#include "commands/GenericControllableCommand.h"

ChataigneGenericModule::ChataigneGenericModule() :
	Module("Generic")
{
	defManager.add(CommandDefinition::createDef(this, "", "Set Parameter Value", &GenericControllableCommand::create)->addParam("action",GenericControllableCommand::SET_VALUE));
	defManager.add(CommandDefinition::createDef(this, "", "Trigger a Control", &GenericControllableCommand::create)->addParam("action",GenericControllableCommand::TRIGGER));
}

ChataigneGenericModule::~ChataigneGenericModule()
{
}
