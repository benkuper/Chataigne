/*
  ==============================================================================

	QLabModule.cpp
	Created: 10 May 2020 6:30:00pm
	Author:  jgastonraoul

  ==============================================================================
*/

#include "QLabModule.h"
#include "../commands/OSCCommand.h"

QLabModule::QLabModule() :
	OSCModule(getDefaultTypeString(),53001,53000,false,true)
{
//WORKSPACE
    var cueNumberArgs = var();
    cueNumberArgs.append(ControllableUtil::createDataForParam(StringParameter::getTypeStringStatic(), "Cue Number", "Number of the Cue", ""));
    var panicTimeArgs = var();
    panicTimeArgs.append(ControllableUtil::createDataForParam(FloatParameter::getTypeStringStatic(), "Panic Time", "Duration of Panic",  0., 0., 1000.));

    defManager->add(CommandDefinition::createDef(this, "Workspace", "Go", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/go")->addParam("args", cueNumberArgs));
    defManager->add(CommandDefinition::createDef(this, "Workspace", "Panic", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/panic"));
    defManager->add(CommandDefinition::createDef(this, "Workspace", "Panic In Time", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/panicInTime")->addParam("args", panicTimeArgs));
    defManager->add(CommandDefinition::createDef(this, "Workspace", "Pause", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/pause"));
    defManager->add(CommandDefinition::createDef(this, "Workspace", "Reset", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/reset"));
    defManager->add(CommandDefinition::createDef(this, "Workspace", "Resume", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/resume"));
}
