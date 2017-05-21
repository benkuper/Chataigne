/*
  ==============================================================================

    ResolumeOutput.cpp
    Created: 29 Oct 2016 7:21:01pm
    Author:  bkupe

  ==============================================================================
*/

#include "ResolumeModule.h"
#include "CommandFactory.h"
#include "ResolumeClipCommand.h"
#include "ResolumeFXCommand.h"

ResolumeModule::ResolumeModule() :
	OSCModule(getDefaultTypeString(),7001,7000)
{	
	var stopArgs = var();
	stopArgs.append(ControllableUtil::createDataForParam(IntParameter::getTypeStringStatic(),"Value","Resolume needs this arg to trigger",1,0,1,false,true));

	defManager.add(CommandDefinition::createDef(this, "Composition", "Stop Composition", &OSCCommand::create, CommandContext::ACTION)->addParam("address","/composition/disconnectall")->addParam("args",stopArgs));	
	
	defManager.add(CommandDefinition::createDef(this, "Launch / Stop", "Launch Clip", &ResolumeClipCommand::create, CommandContext::ACTION)->addParam("level", "Clip")->addParam("suffix","connect"));
	defManager.add(CommandDefinition::createDef(this, "Launch / Stop", "Stop Layer", &ResolumeClipCommand::create, CommandContext::ACTION)->addParam("level", "Layer")->addParam("suffix","clear"));
	defManager.add(CommandDefinition::createDef(this, "Launch / Stop", "Launch MultiClip", &ResolumeClipCommand::create, CommandContext::ACTION)->addParam("level", "Clip")->addParam("suffix", "connect")->addParam("multi",true));
	defManager.add(CommandDefinition::createDef(this, "Launch / Stop", "Launch Column", &ResolumeClipCommand::create, CommandContext::ACTION)->addParam("level", "Column")->addParam("suffix", "connect"));
	defManager.add(CommandDefinition::createDef(this, "Launch / Stop", "Launch MultiColumn", &ResolumeClipCommand::create, CommandContext::ACTION)->addParam("level", "Column")->addParam("suffix", "connect")->addParam("multi", true));
	
	defManager.add(CommandDefinition::createDef(this, "Effects", "Video Parameter", &ResolumeFXCommand::create, CommandContext::BOTH)->addParam("fxType", "transform"));
	defManager.add(CommandDefinition::createDef(this, "Effects", "Video FX", &ResolumeFXCommand::create, CommandContext::BOTH)->addParam("fxType", "videofx"));
	defManager.add(CommandDefinition::createDef(this, "Effects", "Audio Parameter", &ResolumeFXCommand::create, CommandContext::BOTH)->addParam("fxType", "audio"));
	defManager.add(CommandDefinition::createDef(this, "Effects", "Audio FX", &ResolumeFXCommand::create, CommandContext::BOTH)->addParam("fxType", "vst"));
	defManager.add(CommandDefinition::createDef(this, "Effects", "Source Parameter", &ResolumeFXCommand::create, CommandContext::BOTH)->addParam("fxType", "source"));

}
