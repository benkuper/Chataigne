/*
==============================================================================

MilluminModule.cpp
Created: 17 May 2017 7:41:53pm
Author:  Ben

==============================================================================
*/

#include "VLCModule.h"
#include "../../common/streaming/commands/SendStreamStringCommand.h"

VLCModule::VLCModule() :
	TCPModule(getDefaultTypeString(),50000)
{

	NLOG("VLC", "Notice:\nTo be able to control VLC via Remote Control, you must launch the app with rc options:\nExample for launching in windows on port 50000 :\nvlc.exe --rc-host localhost:50000");

	defManager.add(CommandDefinition::createDef(this, "Media", "Play", &SendStreamStringCommand::create, CommandContext::ACTION)->addParam("fixedValue","play"));
	defManager.add(CommandDefinition::createDef(this, "Media", "Stop", &SendStreamStringCommand::create, CommandContext::ACTION)->addParam("fixedValue","stop"));
	defManager.add(CommandDefinition::createDef(this, "Media", "Pause", &SendStreamStringCommand::create, CommandContext::ACTION)->addParam("fixedValue", "pause"));
	defManager.add(CommandDefinition::createDef(this, "Media", "Rewind", &SendStreamStringCommand::create, CommandContext::ACTION)->addParam("fixedValue", "rewind"));
	defManager.add(CommandDefinition::createDef(this, "Media", "Faster Speed", &SendStreamStringCommand::create, CommandContext::ACTION)->addParam("fixedValue", "faster"));
	defManager.add(CommandDefinition::createDef(this, "Media", "Slower Speed", &SendStreamStringCommand::create, CommandContext::ACTION)->addParam("fixedValue", "slower"));
	defManager.add(CommandDefinition::createDef(this, "Media", "Normal Speed", &SendStreamStringCommand::create, CommandContext::ACTION)->addParam("fixedValue", "normal"));
	defManager.add(CommandDefinition::createDef(this, "Media", "Frame by frame", &SendStreamStringCommand::create, CommandContext::ACTION)->addParam("fixedValue", "frame"));
	defManager.add(CommandDefinition::createDef(this, "Media", "Next", &SendStreamStringCommand::create, CommandContext::ACTION)->addParam("fixedValue", "next"));
	defManager.add(CommandDefinition::createDef(this, "Media", "Prev", &SendStreamStringCommand::create, CommandContext::ACTION)->addParam("fixedValue", "prev"));

	defManager.add(CommandDefinition::createDef(this, "Playlist", "Clear", &SendStreamStringCommand::create, CommandContext::ACTION)->addParam("fixedValue", "clear"));
}