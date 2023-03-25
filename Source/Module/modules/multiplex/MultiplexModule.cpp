/*
  ==============================================================================

	MultiplexModule.cpp
	Created: 19 Feb 2021 5:57:00pm
	Author:  bkupe

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

MultiplexModule::MultiplexModule() :
	Module("Multiplex")
{
	defManager->add(CommandDefinition::createDef(this, "", "Set List Value", &MultiplexCommand::create)->addParam("action", GenericControllableCommand::SET_VALUE));

	defManager->add(CommandDefinition::createDef(this, "", "Go to List Value", &MultiplexCommand::create)->addParam("action", GenericControllableCommand::GO_TO_VALUE));

	defManager->add(CommandDefinition::createDef(this, "", "Trigger List", &MultiplexCommand::create)->addParam("action", GenericControllableCommand::TRIGGER));
}

MultiplexModule::~MultiplexModule()
{
}
