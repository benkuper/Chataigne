/*
  ==============================================================================

    LiveOSCModule.cpp
    Created: 13 Apr 2017 11:02:29am
    Author:  Ben

  ==============================================================================
*/

#include "LiveOSCModule.h"
#include "commands/LiveOSCSceneCommand.h"
#include "commands/LiveOSCCommandBase.h"

LiveOSCModule::LiveOSCModule() :
	OSCModule(getDefaultTypeString(), 9000, 9001, false, true)
{
	defManager.add(CommandDefinition::createDef(this, "General", "Play", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/live/play"));
	defManager.add(CommandDefinition::createDef(this, "General", "Continue", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/live/play/continue"));
	defManager.add(CommandDefinition::createDef(this, "General", "Stop", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/live/stop"));

	var floatValueArgs = var();
	var floatArg = ControllableUtil::createDataForParam(FloatParameter::getTypeStringStatic(), "Tempo", "Target Tempo", 0, 20,999);
	floatArg.getDynamicObject()->setProperty("mappingIndex", 0);
	floatValueArgs.append(floatArg);
	defManager.add(CommandDefinition::createDef(this, "General", "Set Tempo", &OSCCommand::create, CommandContext::BOTH)->addParam("address", "/live/tempo")->addParam("args", floatValueArgs));
	
	floatValueArgs = var();
	floatArg = ControllableUtil::createDataForParam(FloatParameter::getTypeStringStatic(), "Time", "Target Time", 0, 0, 600);
	floatArg.getDynamicObject()->setProperty("mappingIndex", 0);
	floatValueArgs.append(floatArg); 
	defManager.add(CommandDefinition::createDef(this, "General", "Set Time", &OSCCommand::create, CommandContext::BOTH)->addParam("address", "/live/time")->addParam("args",floatValueArgs));
	defManager.add(CommandDefinition::createDef(this, "Scene", "Play Scene", &LiveOSCSceneCommand::create, CommandContext::ACTION));

	defManager.add(CommandDefinition::createDef(this, "Track", "Set Volume", &LiveOSCCommandBase::create, CommandContext::BOTH)->addParam("type", LiveOSCCommandBase::VOLUME));
	defManager.add(CommandDefinition::createDef(this, "Track", "Set Mute", &LiveOSCCommandBase::create, CommandContext::BOTH)->addParam("type", LiveOSCCommandBase::MUTE));
	defManager.add(CommandDefinition::createDef(this, "Track", "Set Armed", &LiveOSCCommandBase::create, CommandContext::BOTH)->addParam("type", LiveOSCCommandBase::ARM));
	defManager.add(CommandDefinition::createDef(this, "Track", "Set Pan", &LiveOSCCommandBase::create, CommandContext::BOTH)->addParam("type", LiveOSCCommandBase::PAN));
	defManager.add(CommandDefinition::createDef(this, "Track", "Set Solo", &LiveOSCCommandBase::create, CommandContext::BOTH)->addParam("type", LiveOSCCommandBase::SOLO));
	defManager.add(CommandDefinition::createDef(this, "Track", "Set Send", &LiveOSCCommandBase::create, CommandContext::BOTH)->addParam("type", LiveOSCCommandBase::SEND));
	defManager.add(CommandDefinition::createDef(this, "Track", "Stop", &LiveOSCCommandBase::create, CommandContext::ACTION)->addParam("type", LiveOSCCommandBase::TRACK_STOP));

	defManager.add(CommandDefinition::createDef(this, "Device", "Set Enabled", &LiveOSCCommandBase::create, CommandContext::BOTH)->addParam("type", LiveOSCCommandBase::DEVICE_ENABLE));
	defManager.add(CommandDefinition::createDef(this, "Device", "Set Parameter", &LiveOSCCommandBase::create, CommandContext::BOTH)->addParam("type", LiveOSCCommandBase::DEVICE_PARAM));

	defManager.add(CommandDefinition::createDef(this, "Clip", "Play Clip", &LiveOSCCommandBase::create, CommandContext::ACTION)->addParam("type", LiveOSCCommandBase::CLIP_PLAY));
	defManager.add(CommandDefinition::createDef(this, "Clip", "Stop Clip", &LiveOSCCommandBase::create, CommandContext::ACTION)->addParam("type", LiveOSCCommandBase::CLIP_STOP));
}