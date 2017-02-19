/*
  ==============================================================================

    ResolumeOutput.cpp
    Created: 29 Oct 2016 7:21:01pm
    Author:  bkupe

  ==============================================================================
*/

#include "ResolumeModule.h"
#include "ResolumeClipCommand.h"
#include "ResolumeBaseCommand.h"
#include "CommandFactory.h"

ResolumeModule::ResolumeModule() :
	OSCModule("Resolume",7001,7000)
{	
/*
	"Composition"
		"Stop Composition"

	"Clip Transport"
		"Launch Clip"
		"Launch MultiClip"
		"Stop Clip"

	"Column Transport"
		"Launch Column"
		"Launch MultiColumn"
		"Stop Column"

	"Video"
		"Video Opacity"
		"Video Parameter"
		"Source Parameter"
		"Dashboard Parameter"

	"Audio"
		"Audio Volume"
		"Audio Parameter"
*/
	defManager.add(CommandDefinition::createDef(this, "Composition", "Stop Composition", &OSCCommand::create, CommandContext::ACTION)->addParam("level","Composition")->addParam("suffix","stop"));	
	defManager.add(CommandDefinition::createDef(this, "Clip & Layer", "Launch Clip", &ResolumeClipCommand::create, CommandContext::ACTION)->addParam("level", "Clip")->addParam("suffix","connect"));
	defManager.add(CommandDefinition::createDef(this, "Clip & Layer", "Stop Layer", &ResolumeClipCommand::create, CommandContext::ACTION)->addParam("level", "Layer")->addParam("suffix","clear"));

}
