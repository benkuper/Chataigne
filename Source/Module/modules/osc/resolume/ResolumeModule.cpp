/*
  ==============================================================================

    ResolumeOutput.cpp
    Created: 29 Oct 2016 7:21:01pm
    Author:  bkupe

  ==============================================================================
*/

#include "ResolumeModule.h"
#include "Common/Command/CommandFactory.h"
#include "commands/ResolumeClipCommand.h"
#include "commands/ResolumeFXCommand.h"

ResolumeModule::ResolumeModule() :
	OSCModule(getDefaultTypeString(),7001,7000,false,true)
{	

	version = moduleParams.addEnumParameter("Resolume Version", "Version of Resolume");
	version->addOption("Resolume 6.0.9+", 6.09f)->addOption("Resolume 6", 6)->addOption("Resolume 5", 5);

	var stopArgs = var();
	stopArgs.append(ControllableUtil::createDataForParam(IntParameter::getTypeStringStatic(),"Value","Resolume needs this arg to trigger",1,0,1,false,true));

	defManager->add(CommandDefinition::createDef(this, "Composition", "Stop Composition", &OSCCommand::create, CommandContext::ACTION)->addParam("address","/composition/disconnectall")->addParam("args",stopArgs));	
	
	defManager->add(CommandDefinition::createDef(this, "Launch / Stop", "Launch Clip", &ResolumeClipCommand::create, CommandContext::BOTH)->addParam("level", ResolumeClipCommand::CLIP)->addParam("suffix", "connect"));
	defManager->add(CommandDefinition::createDef(this, "Launch / Stop", "Stop Layer", &ResolumeClipCommand::create, CommandContext::BOTH)->addParam("level", ResolumeClipCommand::LAYER)->addParam("suffix", "clear")->addParam("needsOnOff", true));
	defManager->add(CommandDefinition::createDef(this, "Launch / Stop", "Launch MultiClip", &ResolumeClipCommand::create, CommandContext::BOTH)->addParam("level", ResolumeClipCommand::CLIP)->addParam("suffix", "connect")->addParam("multi",true));
	defManager->add(CommandDefinition::createDef(this, "Launch / Stop", "Launch Column", &ResolumeClipCommand::create, CommandContext::BOTH)->addParam("level", ResolumeClipCommand::COLUMN)->addParam("suffix", "connect"));
	defManager->add(CommandDefinition::createDef(this, "Launch / Stop", "Launch MultiColumn", &ResolumeClipCommand::create, CommandContext::BOTH)->addParam("level", ResolumeClipCommand::COLUMN)->addParam("suffix", "connect")->addParam("multi", true));
	
	defManager->add(CommandDefinition::createDef(this, "Effects", "Video Parameter", &ResolumeFXCommand::create, CommandContext::BOTH)->addParam("fxType", "video"));
	defManager->add(CommandDefinition::createDef(this, "Effects", "Video FX", &ResolumeFXCommand::create, CommandContext::BOTH)->addParam("fxType", "videofx"));
	defManager->add(CommandDefinition::createDef(this, "Effects", "Audio Parameter", &ResolumeFXCommand::create, CommandContext::BOTH)->addParam("fxType", "audio"));
	defManager->add(CommandDefinition::createDef(this, "Effects", "Audio FX", &ResolumeFXCommand::create, CommandContext::BOTH)->addParam("fxType", "vst"));
	defManager->add(CommandDefinition::createDef(this, "Effects", "Source Parameter", &ResolumeFXCommand::create, CommandContext::BOTH)->addParam("fxType", "source"));

	defManager->add(CommandDefinition::createDef(this, "Selection", "Select Clip", &ResolumeClipCommand::create, CommandContext::BOTH)->addParam("level", ResolumeClipCommand::CLIP)->addParam("suffix", "select"));
	defManager->add(CommandDefinition::createDef(this, "Selection", "Select Layer", &ResolumeClipCommand::create, CommandContext::BOTH)->addParam("level", ResolumeClipCommand::LAYER)->addParam("suffix", "select"));
	defManager->add(CommandDefinition::createDef(this, "Selection", "Select Deck", &ResolumeClipCommand::create, CommandContext::BOTH)->addParam("level", ResolumeClipCommand::DECK)->addParam("suffix", "select"));
}

/*
InspectableEditor * ResolumeModule::getEditor(bool isRoot)
{
	return new ResolumeModuleEditor(this,isRoot);
}
*/
