/*
  ==============================================================================

    ReaperModule.cpp
    Created: 26 Jan 2017 2:39:33pm
    Author:  Ben

  ==============================================================================
*/

#include "ReaperModule.h"
#include "OSCCommand.h"
#include "ReaperTimeCommand.h"
ReaperModule::ReaperModule() :
	OSCModule("Reaper",9000,8000,false,true)
{
	sendCC->editorIsCollapsed = false;

	defManager.add(CommandDefinition::createDef(this, "", "Play", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/play"));
	defManager.add(CommandDefinition::createDef(this, "", "Pause", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/pause"));
	defManager.add(CommandDefinition::createDef(this, "", "Stop", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/stop"));
	defManager.add(CommandDefinition::createDef(this, "", "Set Time", &ReaperTimeCommand::create, CommandContext::BOTH)->addParam("address", "/time"));
}
