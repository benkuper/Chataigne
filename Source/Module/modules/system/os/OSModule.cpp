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
	Module(getDefaultTypeString()),
	Thread("OS-ChildProcess")
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
	scriptObject.setMethod(launchCommandId, &OSModule::launchCommandFromScript);
	scriptObject.setMethod(launchProcessId, &OSModule::launchProcessFromScript);

	startTimer(5000);
}

OSModule::~OSModule()
{
	stopTimer();
	signalThreadShouldExit();
	waitForThreadToExit(100);
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

void OSModule::launchCommand(const String& command, bool silentMode)
{
	int result = 0;
#if JUCE_WINDOWS
	if (silentMode) WinExec(command.getCharPointer(), SW_HIDE);
	else result = system(command.getCharPointer());
#else
	result = system(command.getCharPointer());
#endif
	if (logOutgoingData->boolValue()) NLOG(niceName, "Launching : " + command);
	if (result != 0) NLOGERROR(niceName, "Error trying to launch command : " << command);
	outActivityTrigger->trigger();
}

void OSModule::launchChildProcess(const String& command)
{
	signalThreadShouldExit();
	waitForThreadToExit(100); 
	commandToRun = command;
	startThread();
}

String OSModule::launchChildProcessBlocking(const String& command)
{
	childProcess.reset(new ChildProcess());
	if (commandToRun.isNotEmpty()) childProcess->start(commandToRun);
	return childProcess->readAllProcessOutput();
}

var OSModule::launchFileFromScript(const var::NativeFunctionArgs& args)
{
	OSModule* m = getObjectFromJS<OSModule>(args);
	if (!m->enabled->boolValue()) return var();

	if (!checkNumArgs(m->niceName, args, 1)) return var();

	File f = File(args.arguments[0]);
	bool result = m->launchFile(f, args.numArguments > 1 ? args.arguments[1].toString():"");

	return result;
}

var OSModule::launchCommandFromScript(const var::NativeFunctionArgs& args)
{
	OSModule* m = getObjectFromJS<OSModule>(args);
	if (!m->enabled->boolValue()) return var();

	if (!checkNumArgs(m->niceName, args, 1)) return var();

	m->launchCommand(args.arguments[0].toString(), args.numArguments > 1 ? (int)args.arguments[1] > 0:false);
	return var();
}

var OSModule::launchProcessFromScript(const var::NativeFunctionArgs& args)
{
	OSModule* m = getObjectFromJS<OSModule>(args);
	if (!m->enabled->boolValue()) return var();

	if (!checkNumArgs(m->niceName, args, 1)) return var();

	if (args.numArguments > 1 && (int)args.arguments[1] > 0) return m->launchChildProcessBlocking(args.arguments[0].toString());
	else m->launchChildProcess(args.arguments[0]);

	return var();
}

void OSModule::timerCallback()
{
	updateIps();
}

void OSModule::run()
{
	if (childProcess != nullptr && childProcess->isRunning()) childProcess->kill();
	childProcess.reset(new ChildProcess());
	if(commandToRun.isNotEmpty()) childProcess->start(commandToRun);

	char buffer[1024];
	while (!threadShouldExit() && childProcess->isRunning())
	{
		int numRead = childProcess->readProcessOutput(buffer, 1024);
		if (numRead > 0)
		{
			String s(buffer, numRead);
			scriptManager->callFunctionOnAllItems("processDataReceived", s);
		}
	}

	if(childProcess != nullptr && childProcess->isRunning()) childProcess->kill();
}
