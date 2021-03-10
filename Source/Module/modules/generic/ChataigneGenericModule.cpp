/*
  ==============================================================================

    ChataigneGenericModule.cpp
    Created: 28 Apr 2017 5:50:41pm
    Author:  Ben

  ==============================================================================
*/

ChataigneGenericModule::ChataigneGenericModule() :
	Module("Generic")
{
	defManager->add(CommandDefinition::createDef(this, "", "Set Parameter Value", &GenericControllableCommand::create)->addParam("action", GenericControllableCommand::SET_VALUE));
	defManager->add(CommandDefinition::createDef(this, "", "Trigger a Control", &GenericControllableCommand::create)->addParam("action", GenericControllableCommand::TRIGGER));
	defManager->add(CommandDefinition::createDef(this, "", "Log a message", &ChataigneLogCommand::create, CommandContext::ACTION)->addParam("type", ChataigneLogCommand::MESSAGE));
	defManager->add(CommandDefinition::createDef(this, "", "Log a value", &ChataigneLogCommand::create)->addParam("type", ChataigneLogCommand::VALUE));
	defManager->add(CommandDefinition::createDef(this, "", "Script", &GenericScriptCommand::create));
	defManager->add(CommandDefinition::createDef(this, "", "New Session", &GenericAppCommand::create, CommandContext::ACTION)->addParam("type", GenericAppCommand::NEW_SESSION));
	defManager->add(CommandDefinition::createDef(this, "", "Open Session", &GenericAppCommand::create, CommandContext::ACTION)->addParam("type", GenericAppCommand::OPEN_SESSION));
	defManager->add(CommandDefinition::createDef(this, "", "Close App", &GenericAppCommand::create, CommandContext::ACTION)->addParam("type", GenericAppCommand::CLOSE_APP));
	defManager->add(CommandDefinition::createDef(this, "", "Minimize Window", &GenericAppCommand::create, CommandContext::ACTION)->addParam("type", GenericAppCommand::MINIMIZE));
	defManager->add(CommandDefinition::createDef(this, "", "Maximize Window", &GenericAppCommand::create, CommandContext::ACTION)->addParam("type", GenericAppCommand::MAXIMIZE));
	defManager->add(CommandDefinition::createDef(this, "", "Close to Tray", &GenericAppCommand::create, CommandContext::ACTION)->addParam("type", GenericAppCommand::CLOSE_TO_TRAY));
	defManager->add(CommandDefinition::createDef(this, "", "Open from Tray", &GenericAppCommand::create, CommandContext::ACTION)->addParam("type", GenericAppCommand::OPEN_FROM_TRAY));
}

ChataigneGenericModule::~ChataigneGenericModule()
{
}
