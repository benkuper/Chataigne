/*
  ==============================================================================

    ChataigneGenericModule.cpp
    Created: 28 Apr 2017 5:50:41pm
    Author:  Ben

  ==============================================================================
*/

#include "ChataigneGenericModule.h"
#include "GenericSetParameterCommand.h"
ChataigneGenericModule::ChataigneGenericModule() :
	Module("Generic")
{
	defManager.add(CommandDefinition::createDef(this, "", "Set Parameter Value", &GenericSetParameterCommand::create));
}

ChataigneGenericModule::~ChataigneGenericModule()
{
}
