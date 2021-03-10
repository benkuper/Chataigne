/*
  ==============================================================================

    WatchoutModule.cpp
    Created: 2 Jan 2018 11:54:57am
    Author:  Ben

  ==============================================================================
*/

WatchoutModule::WatchoutModule() :
	TCPClientModule("Watchout",3040)
{
	streamingType->hideInEditor = true;
	streamingType->setValueWithData(LINES);

	defManager->add(getBaseWCommand("","Ping","ping"));
	defManager->add(getBaseWCommand("", "Load", "load", "s:\"File Path\""));
	defManager->add(getBaseWCommand("", "Online", "online", "b:Value"));
	defManager->add(getBaseWCommand("", "Run", "run"));
	defManager->add(getBaseWCommand("", "Halt", "halt"));
	defManager->add(getBaseWCommand("", "Go To Time", "gotoTime", "i:Time"));
	defManager->add(getBaseWCommand("", "Go To Cue", "gotoControlCue", "s:Cue"));
	defManager->add(getBaseWCommand("", "Stand By", "standBy", "b:Value i:Fade"));

}

WatchoutModule::~WatchoutModule()
{
}

CommandDefinition * WatchoutModule::getBaseWCommand(const String & menu, const String & commandName, const String & command, const String &params, CommandContext context)
{

	return CommandDefinition::createDef(this,menu,commandName, &WatchoutCommand::create, context)->addParam("fixedValue", command)->addParam("forceCR", true)->addParam("forceNL", true)->addParam("args",params);
}
