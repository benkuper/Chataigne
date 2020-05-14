/*
  ==============================================================================

	QLabModule.cpp
	Created: 10 May 2020 6:30:00pm
	Author:  jgastonraoul

  ==============================================================================
*/

#include "QLabModule.h"
#include "commands/QLabWorkspaceCommand.h"

QLabModule::QLabModule() :
	OSCModule(getDefaultTypeString(),53001,53000,false,true)
{
    workspaceID = moduleParams.addStringParameter("Workspace ID", "The ID of the workspace (leave empty if not defined)", "");

//WORKSPACE
    var cueNumberArgs = var();
    cueNumberArgs.append(ControllableUtil::createDataForParam(StringParameter::getTypeStringStatic(), "Cue Number", "Number of the Cue", ""));
    var panicTimeArgs = var();
    panicTimeArgs.append(ControllableUtil::createDataForParam(FloatParameter::getTypeStringStatic(), "Panic Time", "Duration of Panic",  0., 0., 1000.));

    var testArgs = var();
    var enumT = ControllableUtil::createDataForParam(EnumParameter::getTypeStringStatic(), "Test enum", "Test enum", 0);
    var options = new DynamicObject();
    options.getDynamicObject()->setProperty("Int value", 1);
    options.getDynamicObject()->setProperty("String value", "stringval");
    options.getDynamicObject()->setProperty("Float value", 2.1);
    enumT.getDynamicObject()->setProperty("options", options);
    testArgs.append(enumT);

    defManager->add(CommandDefinition::createDef(this, "Workspace", "Go", &QLabWorkspaceCommand::create, CommandContext::ACTION)->addParam("address", "/go")->addParam("args", cueNumberArgs));
    defManager->add(CommandDefinition::createDef(this, "Workspace", "Panic", &QLabWorkspaceCommand::create, CommandContext::ACTION)->addParam("address", "/panic"));
    defManager->add(CommandDefinition::createDef(this, "Workspace", "Panic In Time", &QLabWorkspaceCommand::create, CommandContext::ACTION)->addParam("address", "/panicInTime")->addParam("args", panicTimeArgs));
    defManager->add(CommandDefinition::createDef(this, "Workspace", "Pause", &QLabWorkspaceCommand::create, CommandContext::ACTION)->addParam("address", "/pause"));
    defManager->add(CommandDefinition::createDef(this, "Workspace", "Reset", &QLabWorkspaceCommand::create, CommandContext::ACTION)->addParam("address", "/reset"));
    defManager->add(CommandDefinition::createDef(this, "Workspace", "Resume", &QLabWorkspaceCommand::create, CommandContext::ACTION)->addParam("address", "/resume"));

    defManager->add(CommandDefinition::createDef(this, "Test", "test enum", &QLabWorkspaceCommand::create, CommandContext::ACTION)->addParam("address", "/test")->addParam("args", testArgs));

}

String QLabModule::getWorkspacePrefix() const
{
    String wid = workspaceID->stringValue();
    return wid.isEmpty() ? "" : "/workspace/" + wid;
}

String QLabModule::getAddressForRoutedValue(Controllable* c, OSCRouteParams* op)
{
    return getWorkspacePrefix() + OSCModule::getAddressForRoutedValue(c, op);
}
