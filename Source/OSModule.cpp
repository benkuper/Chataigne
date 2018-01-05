/*
  ==============================================================================

    OSModule.cpp
    Created: 5 Jan 2018 3:41:34pm
    Author:  Ben

  ==============================================================================
*/

#include "OSModule.h"
#include "OSPowerCommand.h"
#include "OSExecCommand.h"

OSModule::OSModule() :
	Module(getDefaultTypeString())
{
	setupIOConfiguration(false, true);

	osType = valuesCC.addEnumParameter("OS Type", "Type of OS");
	osType->addOption("Windows", OS_WIN)->addOption("MacOS", OS_MAC)->addOption("Linux", OS_LINUX);
#if JUCE_WINDOWS
	osType->setValueWithData(OS_WIN);
#elif JUCE_MAC
	osType->setValueWithData(OS_MAC);
#elif JUCE_LINUX
	osType->setValueWithData(OS_LINUX);
#endif

	osName = valuesCC.addStringParameter("OS Name", "Name of the OS", SystemStats::getOperatingSystemName());
	moduleParams.hideInEditor = true;

	for (auto &c : valuesCC.controllables) c->isControllableFeedbackOnly = true;


	defManager.add(CommandDefinition::createDef(this, "Power", "Shutdown", &OSPowerCommand::create, CommandContext::ACTION)->addParam("type", OSPowerCommand::SHUTDOWN));
	defManager.add(CommandDefinition::createDef(this, "Power", "Reboot", &OSPowerCommand::create, CommandContext::ACTION)->addParam("type", OSPowerCommand::REBOOT));
	
	defManager.add(CommandDefinition::createDef(this, "Process", "Open File", &OSExecCommand::create, CommandContext::ACTION)->addParam("type", OSExecCommand::OPEN_FILE));
	defManager.add(CommandDefinition::createDef(this, "Process", "Launch App", &OSExecCommand::create, CommandContext::ACTION)->addParam("type", OSExecCommand::LAUNCH_APP));
	defManager.add(CommandDefinition::createDef(this, "Process", "Kill App", &OSExecCommand::create, CommandContext::ACTION)->addParam("type", OSExecCommand::KILL_APP));
}

OSModule::~OSModule()
{

}