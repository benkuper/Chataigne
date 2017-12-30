/*
  ==============================================================================

    LiveOSCModule.cpp
    Created: 13 Apr 2017 11:02:29am
    Author:  Ben

  ==============================================================================
*/

#include "LiveOSCModule.h"
#include "OSCCommand.h"
#include "LiveOSCSceneCommand.h"
#include "LiveOSCVolumeCommand.h"
#include "LiveOSCDeviceCommand.h"
#include "LiveOSCCommandBase.h"

LiveOSCModule::LiveOSCModule() :
	OSCModule("LiveOSC", 9001, 9000,false,true)
{

	//currentScene = addIntParameter("Current Scene", "Current Scene", 0,0, 1000);

	defManager.add(CommandDefinition::createDef(this, "General", "Play", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/live/play"));
	defManager.add(CommandDefinition::createDef(this, "General", "Stop", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/live/stop"));
	defManager.add(CommandDefinition::createDef(this, "Scene", "Play Scene", &LiveOSCSceneCommand::create, CommandContext::ACTION)->addParam("address", "/live/play/scene"));
	defManager.add(CommandDefinition::createDef(this, "Track", "Set Volume", &LiveOSCVolumeCommand::create, CommandContext::BOTH)->addParam("address", "/live/master/volume")->addParam("target","master"));
	defManager.add(CommandDefinition::createDef(this, "Track", "Set Mute", &LiveOSCCommandBase::create, CommandContext::BOTH)->addParam("address", "/live/mute"));
	defManager.add(CommandDefinition::createDef(this, "Device", "Toggle Enable", &LiveOSCDeviceCommand::create, CommandContext::BOTH)->addParam("address", "/live/master/device")->addParam("paramID", 0));
}