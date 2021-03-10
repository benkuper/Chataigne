/*
  ==============================================================================

    MultiplexModule.cpp
    Created: 19 Feb 2021 5:57:00pm
    Author:  bkupe

  ==============================================================================
*/

MultiplexModule::MultiplexModule() :
	Module("Multiplex")
{
    defManager->add(CommandDefinition::createDef(this, "", "Set List Value", &MultiplexCommand::create)->addParam("type", MultiplexCommand::SET_LIST_VALUE));
}

MultiplexModule::~MultiplexModule()
{
}
