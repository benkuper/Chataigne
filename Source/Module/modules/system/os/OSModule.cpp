/*
  ==============================================================================

    OSModule.cpp
    Created: 5 Jan 2018 3:41:34pm
    Author:  Ben

  ==============================================================================
*/

#include "OSModule.h"
#include "commands/OSPowerCommand.h"
#include "commands/OSExecCommand.h"
#include "commands/WakeOnLanCommand.h"
#include "commands/OSWindowCommand.h"

OSModule::OSModule() :
	Module(getDefaultTypeString())
{

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

	ips = valuesCC.addStringParameter("IP", "IP that has been detected than most probable to be a LAN IP", NetworkHelpers::getLocalIP());

	for (auto &c : valuesCC.controllables) c->isControllableFeedbackOnly = true;

	setupIOConfiguration(false, true);

	defManager->add(CommandDefinition::createDef(this, "Power", "Shutdown", &OSPowerCommand::create, CommandContext::ACTION)->addParam("type", OSPowerCommand::SHUTDOWN));
	defManager->add(CommandDefinition::createDef(this, "Power", "Reboot", &OSPowerCommand::create, CommandContext::ACTION)->addParam("type", OSPowerCommand::REBOOT));
	defManager->add(CommandDefinition::createDef(this, "Power", "Wake On Lan", &WakeOnLanCommand::create, CommandContext::ACTION));
	
	defManager->add(CommandDefinition::createDef(this, "Process", "Open File", &OSExecCommand::create, CommandContext::ACTION)->addParam("type", OSExecCommand::OPEN_FILE));
	defManager->add(CommandDefinition::createDef(this, "Process", "Launch App", &OSExecCommand::create, CommandContext::ACTION)->addParam("type", OSExecCommand::LAUNCH_APP));
	defManager->add(CommandDefinition::createDef(this, "Process", "Launch Command", &OSExecCommand::create, CommandContext::ACTION)->addParam("type", OSExecCommand::LAUNCH_COMMAND));
	defManager->add(CommandDefinition::createDef(this, "Process", "Launch Command File", &OSExecCommand::create, CommandContext::ACTION)->addParam("type", OSExecCommand::LAUNCH_COMMAND_FILE));
	defManager->add(CommandDefinition::createDef(this, "Process", "Kill App", &OSExecCommand::create, CommandContext::ACTION)->addParam("type", OSExecCommand::KILL_APP));
	defManager->add(CommandDefinition::createDef(this, "Window", "Set Window Parameters", &OSWindowCommand::create, CommandContext::ACTION));

	scriptObject.setMethod(launchAppId, &OSModule::launchFileFromScript);

	startTimer(5000);
}

OSModule::~OSModule()
{
	stopTimer();
}

void OSModule::updateIps()
{
	ips->setValue(NetworkHelpers::getLocalIP());
}

bool OSModule::launchFile(File f, String args)
{
	if (!f.exists())
	{
		NLOGWARNING(niceName, "File does not exist : " + f.getFullPathName());
		return false;
	}

	File wDir = File::getCurrentWorkingDirectory();
	f.getParentDirectory().setAsCurrentWorkingDirectory();
	bool result = f.startAsProcess(args);
	wDir.setAsCurrentWorkingDirectory();

	if (result) outActivityTrigger->trigger();
	else NLOGERROR(niceName, "Could not launch application " + f.getFullPathName()+" with arguments : "+args);

	return result;
}

var OSModule::launchFileFromScript(const var::NativeFunctionArgs& args)
{
	OSModule* m = getObjectFromJS<OSModule>(args);
	if (!m->enabled->boolValue()) return var();

	if (args.numArguments == 0) return var();

	File f = File(args.arguments[0]);
	bool result = m->launchFile(f, args.numArguments > 1 ? args.arguments[1].toString():"");

	return result;
}

void OSModule::timerCallback()
{
	updateIps();
}
