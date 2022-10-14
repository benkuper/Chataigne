/*
  ==============================================================================

	OSModule.cpp
	Created: 5 Jan 2018 3:41:34pm
	Author:  Ben

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

#if JUCE_WINDOWS
#include <TlHelp32.h>
#endif


OSModule::OSModule() :
	Module(getDefaultTypeString()),
	Thread("OS-ChildProcess"),
	appControlNamesCC("App Control"),
	appControlStatusCC("App Control"),
	pingIPsCC("Ping IPs"),
	pingStatusCC("Ping Status"),
	pingThread(this)
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

	//moduleParams.hideInEditor = true;

	listIPs = moduleParams.addTrigger("List IPs", "List all IPs of all network interfaces");
	pingFrequency = moduleParams.addIntParameter("Ping Frequency", "Time between each ping routine, in seconds.", 5);

	appControlNamesCC.userCanAddControllables = true;
	appControlNamesCC.customUserCreateControllableFunc = std::bind(&OSModule::appControlCreateControllable, this, std::placeholders::_1);
	moduleParams.addChildControllableContainer(&appControlNamesCC);

	pingIPsCC.userCanAddControllables = true;
	pingIPsCC.customUserCreateControllableFunc = std::bind(&OSModule::pingIPsCreateControllable, this, std::placeholders::_1);
	moduleParams.addChildControllableContainer(&pingIPsCC);

	valuesCC.addChildControllableContainer(&appControlStatusCC);
	valuesCC.addChildControllableContainer(&pingStatusCC);

	osName = valuesCC.addStringParameter("OS Name", "Name of the OS", SystemStats::getOperatingSystemName());

	ips = valuesCC.addStringParameter("IP", "IP that has been detected than most probable to be a LAN IP", NetworkHelpers::getLocalIP());

	Array<MACAddress> macList = MACAddress::getAllAddresses();
	String macs = "";
	for (int i = 0; i < macList.size(); i++) macs += (i > 0 ? "\n" : "") + macList[i].toString().replace("-", ":").toUpperCase();

	mac = valuesCC.addStringParameter("MAC", "Mac address of the IP", macs);
	mac->multiline = true;

	terminateTrigger = valuesCC.addTrigger("Terminate", "This will be triggered when the program is about to terminate.");
	crashedTrigger = valuesCC.addTrigger("Crashed", "This will be triggered when the program has crashed, just before killing itself like a sad puppy.");

	for (auto& c : valuesCC.controllables) c->isControllableFeedbackOnly = true;

	setupIOConfiguration(false, true);

	defManager->add(CommandDefinition::createDef(this, "Power", "Shutdown", &OSPowerCommand::create, CommandContext::ACTION)->addParam("type", OSPowerCommand::SHUTDOWN));
	defManager->add(CommandDefinition::createDef(this, "Power", "Reboot", &OSPowerCommand::create, CommandContext::ACTION)->addParam("type", OSPowerCommand::REBOOT));
	defManager->add(CommandDefinition::createDef(this, "Power", "Wake On Lan", &WakeOnLanCommand::create, CommandContext::ACTION));

	defManager->add(CommandDefinition::createDef(this, "Process", "Open File", &OSExecCommand::create, CommandContext::BOTH)->addParam("type", OSExecCommand::OPEN_FILE));
	defManager->add(CommandDefinition::createDef(this, "Process", "Launch App", &OSExecCommand::create, CommandContext::BOTH)->addParam("type", OSExecCommand::LAUNCH_APP));
	defManager->add(CommandDefinition::createDef(this, "Process", "Launch Command", &OSExecCommand::create, CommandContext::BOTH)->addParam("type", OSExecCommand::LAUNCH_COMMAND));
	defManager->add(CommandDefinition::createDef(this, "Process", "Launch Command File", &OSExecCommand::create, CommandContext::BOTH)->addParam("type", OSExecCommand::LAUNCH_COMMAND_FILE));
	defManager->add(CommandDefinition::createDef(this, "Process", "Kill App", &OSExecCommand::create, CommandContext::BOTH)->addParam("type", OSExecCommand::KILL_APP));
	defManager->add(CommandDefinition::createDef(this, "Window", "Set Window Parameters", &OSWindowCommand::create, CommandContext::BOTH));

	scriptObject.setMethod(launchAppId, &OSModule::launchFileFromScript);
	scriptObject.setMethod(launchCommandId, &OSModule::launchCommandFromScript);
	scriptObject.setMethod(launchProcessId, &OSModule::launchProcessFromScript);
	scriptObject.setMethod(getRunningProcessesId, &OSModule::getRunningProcessesFromScript);
	scriptObject.setMethod(isProcessRunningId, &OSModule::isProcessRunningFromScript);


	startTimer(OS_IP_TIMER, 5000);
	startTimer(OS_APP_TIMER, 1000);
}

OSModule::~OSModule()
{
	stopTimer(OS_IP_TIMER);
	stopTimer(OS_APP_TIMER);
	stopThread(100);
	pingThread.stopThread(1000);
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
	else NLOGERROR(niceName, "Could not launch application " + f.getFullPathName() + " with arguments : " + args);

	return result;
}

void OSModule::launchCommand(const String& command, bool silentMode)
{
	int result = 0;
#if JUCE_WINDOWS
	if (silentMode) result = WinExec(command.getCharPointer(), SW_HIDE) > 31 ? 0 : 1;
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
	commandsToRun.add(command);
	startThread();
}

String OSModule::launchChildProcessBlocking(const String& command)
{
	ChildProcess process = ChildProcess();
	process.start(command);
	return process.readAllProcessOutput();
}

var OSModule::launchFileFromScript(const var::NativeFunctionArgs& args)
{
	OSModule* m = getObjectFromJS<OSModule>(args);
	if (!m->enabled->boolValue()) return var();

	if (!checkNumArgs(m->niceName, args, 1)) return var();

	File f = File(args.arguments[0]);
	bool result = m->launchFile(f, args.numArguments > 1 ? args.arguments[1].toString() : "");

	return result;
}

void OSModule::killProcess(const String& processName, bool hardKillMode)
{
#if JUCE_WINDOWS
	int result = WinExec(String("taskkill " + String(hardKillMode ? "/f " : "") + "/im \"" + processName + "\"").getCharPointer(), SW_HIDE);
	if (result < 31) LOGWARNING("Problem killing app " + processName);
#else
	int result = system(String("killall " + String(hardKillMode ? "-9" : "-2") + " \"" + processName + "\"").getCharPointer());
	if (result != 0) LOGWARNING("Problem killing app " + processName);
#endif

	if (result) outActivityTrigger->trigger();
}

void OSModule::checkAppControl()
{
	StringArray processes = getRunningProcesses();
	for (auto& c : appControlStatusCC.controllables)
	{
		String processName = c->niceName;
		bool isRunning = processes.contains(processName);
		((BoolParameter*)c)->setValue(isRunning);
	}
}

var OSModule::launchCommandFromScript(const var::NativeFunctionArgs& args)
{
	OSModule* m = getObjectFromJS<OSModule>(args);
	if (!m->enabled->boolValue()) return var();

	if (!checkNumArgs(m->niceName, args, 1)) return var();

	m->launchCommand(args.arguments[0].toString(), args.numArguments > 1 ? (int)args.arguments[1] > 0:false);
	return var();
}

void OSModule::updateAppControlValues()
{
	appControlStatusCC.clear();

	for (auto& c : appControlNamesCC.controllables)
	{
		File f = ((FileParameter*)c)->getFile();
		BoolParameter* b = appControlStatusCC.addBoolParameter(f.existsAsFile() ? f.getFileName() : "[noapp]", "Status for this process", false);
		b->isSavable = false;
	}
}

void OSModule::updatePingStatusValues()
{
	pingThread.stopThread(1000);
	pingStatusCC.clear();

	for (auto& c : pingIPsCC.controllables)
	{
		String s = ((StringParameter*)c)->stringValue();
		BoolParameter* b = pingStatusCC.addBoolParameter(s.isNotEmpty() ? s : "[noip]", "Status for this IP", false);
		b->isSavable = false;
	}

	if (pingIPsCC.controllables.size() > 0) pingThread.startThread();
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

var OSModule::isProcessRunningFromScript(const var::NativeFunctionArgs& args)
{
	OSModule* m = getObjectFromJS<OSModule>(args);
	if (!checkNumArgs(m->niceName, args, 1)) return var();

	return m->isProcessRunning(args.arguments[0].toString());
}

void OSModule::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	Module::onControllableFeedbackUpdateInternal(cc, c);

	if (c->parentContainer.get() == &appControlNamesCC)
	{
		if (!isCurrentlyLoadingData) updateAppControlValues();
	}
	else if (c->parentContainer.get() == &pingIPsCC)
	{
		if (!isCurrentlyLoadingData) updatePingStatusValues();
	}
	else if (c->parentContainer.get() == &appControlStatusCC)
	{
		bool val = ((BoolParameter*)c)->boolValue();
		bool isRunning = isProcessRunning(c->niceName);

		if (val != isRunning)
		{
			File f = ((FileParameter*)appControlNamesCC.controllables[appControlStatusCC.controllables.indexOf(c)])->getFile();
			if (val) launchFile(f);
			else killProcess(c->niceName, true);
		}
	}
	else if (c == listIPs)
	{
		Array<IPAddress> ad;
		IPAddress::findAllAddresses(ad);
		Array<String> ips;
		for (auto& a : ad) ips.add(a.toString());
		ips.sort();
		String s = "Local IPs:";
		for (auto& ip : ips) s += String("\n > ") + ip;

		NLOG(niceName, s);
	}
}

var OSModule::getRunningProcessesFromScript(const var::NativeFunctionArgs& args)
{
	OSModule* m = getObjectFromJS<OSModule>(args);
	if (!checkNumArgs(m->niceName, args, 1)) return var();

	var result;
	StringArray processes = m->getRunningProcesses();
	for (auto& p : processes) result.append(p);
	return result;
}

void OSModule::appControlCreateControllable(ControllableContainer* c)
{
	FileParameter* fp = c->addFileParameter("App File 1", "App File to watch, launch and kill", "");
	fp->saveValueOnly = false;
	fp->isRemovableByUser = true;

	updateAppControlValues();
}

void OSModule::pingIPsCreateControllable(ControllableContainer* c)
{
	StringParameter* sp = c->addStringParameter("IP 1", "IP to ping", "");
	sp->saveValueOnly = false;
	sp->isRemovableByUser = true;

	updatePingStatusValues();
}


bool OSModule::isProcessRunning(const String& processName)
{
	return getRunningProcesses().contains(processName);
}

StringArray OSModule::getRunningProcesses()
{
	StringArray result;
#if JUCE_WINDOWS
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		LOGERROR("OS List process : Invalid handle value");
	}
	else {
		pe32.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(hProcessSnap, &pe32)) {
			result.add(pe32.szExeFile);
			while (Process32Next(hProcessSnap, &pe32)) result.add(pe32.szExeFile);
		}
	}
	// clean the snapshot object
	CloseHandle(hProcessSnap);
#elif JUCE_MAC
#elif JUCE_LINUX
#endif

	return result;
}

void OSModule::timerCallback(int timerID)
{
	switch (timerID)
	{
	case OS_IP_TIMER: updateIps(); break;
	case OS_APP_TIMER: checkAppControl(); break;
	}
}

void OSModule::run()
{
	while (!threadShouldExit() && !commandsToRun.isEmpty())
	{
		String command = commandsToRun.removeAndReturn(0);
		if (command.isEmpty()) continue;

		ChildProcess process;
		process.start(command);

		char buffer[1024];
		while (!threadShouldExit() && process.isRunning())
		{rs
			int numRead = process.readProcessOutput(buffer, 1024);
			if (numRead > 0)
			{
				String s(buffer, numRead);
				Array<var> args;
				args.add(s);
				args.add(command);
				scriptManager->callFunctionOnAllItems("processDataReceived", args);
			}
		}

		process.kill();
	}
}

void OSModule::afterLoadJSONDataInternal()
{
	Module::afterLoadJSONDataInternal();
	updateAppControlValues();
	updatePingStatusValues();
}


void OSModule::PingThread::run()
{
	while (!threadShouldExit() && !moduleRef.wasObjectDeleted())
	{
		wait(osModule->pingFrequency->intValue() * 1000);

		Array<WeakReference<Parameter>> ipParams = osModule->pingIPsCC.getAllParameters();
		Array<WeakReference<Parameter>> statusParams = osModule->pingStatusCC.getAllParameters();

		if (ipParams.isEmpty()) return;

		for (int i = 0; i < ipParams.size(); i++)
		{
			if (threadShouldExit() || moduleRef.wasObjectDeleted()) return;
			if (ipParams[i].wasObjectDeleted() || statusParams[i].wasObjectDeleted()) continue;
			String ip = ipParams[i]->stringValue();
			if (ip.isEmpty()) continue;

			if (osModule->logOutgoingData->boolValue()) NLOG(osModule->niceName, "Pinging " + ipParams[i]->stringValue() + " ...");

			ChildProcess process;
#if JUCE_WINDOWS
			String command = "ping -n 1 -w 500 " + ip;
#else
			Sring command = "ping -c 1 -W 1 " + ip;
#endif

			process.start(command);

			String result = process.readAllProcessOutput();

#if JUCE_WINDOWS
			bool success = result.contains("Received = 1");
#else
			bool success = result.contains("0% packet loss");
#endif

			if (osModule->logOutgoingData->boolValue())
			{
				if (success) NLOG(osModule->niceName, ipParams[i]->stringValue() << " is alive");
				else NLOGWARNING(osModule->niceName, ipParams[i]->stringValue() << " is dead");
			}

			if (!statusParams[i].wasObjectDeleted()) statusParams[i]->setValue(success);
		}
	}
	
	if (!moduleRef.wasObjectDeleted() && osModule->logOutgoingData->boolValue()) LOG("Stop pinging thread");
}