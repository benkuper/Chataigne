/*
  ==============================================================================

	OSModule.cpp
	Created: 5 Jan 2018 3:41:34pm
	Author:  Ben

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"
#include "lib/cpumem_monitor.h"

#ifndef PING_SUPPORT
#define PING_SUPPORT 1
#endif

#ifndef OS_SYSINFO_SUPPORT
#define OS_SYSINFO_SUPPORT 1
#endif

#if OS_SYSINFO_SUPPORT
#if JUCE_WINDOWS
#include <TlHelp32.h>
#endif
#endif // OS_SYSINFO_SUPPORT


#if PING_SUPPORT
#if JUCE_WINDOWS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <icmpapi.h>
#pragma comment(lib, "ws2_32.lib")
#define CLOSESOCKET closesocket
#else
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#if JUCE_MAC
// If the OS doesn't declare it, do it ourself (copy-pasted from GNU C Library, license: LGPL)
#include <stdint.h>
struct icmphdr
{
	uint8_t type;           /* message type */
	uint8_t code;           /* type sub-code */
	uint16_t checksum;
	union
	{
		struct
		{
			uint16_t        id;
			uint16_t        sequence;
		} echo;                 /* echo datagram */
		uint32_t        gateway;        /* gateway address */
		struct
		{
			uint16_t        unused;
			uint16_t        mtu;
		} frag;                 /* path mtu discovery */
		/*uint8_t reserved[4];*/
	} un;
};

// Fix slightly changed names
#define SOL_IP IPPROTO_IP
#endif
#define CLOSESOCKET close
#endif 
#endif // PING_SUPPORT

float OSModule::timeAtProcessStart = Time::getMillisecondCounter() / 1000.0f;

OSModule::OSModule() :
	Module(getDefaultTypeString()),
	Thread("OS-ChildProcess"),
	osInfoCC("OS Infos"),
	networkInfoCC("Network Infos"),
	appControlNamesCC("App Control"),
	appControlStatusCC("App Control"),
	pingIPsCC("Ping IPs"),
	pingStatusCC("Ping Status"),
	osThread(this),
	pingThread(this)
{
	includeValuesInSave = true;



	//moduleParams.hideInEditor = true;

	//Params
	listIPs = moduleParams.addTrigger("List IPs", "List all IPs of all network interfaces");
	pingInterval = moduleParams.addIntParameter("Ping Interval", "Time between each ping routine, in seconds.", 5);
	pingTimeout = moduleParams.addFloatParameter("Ping Timeout", "Timeout for each ping routine, in seconds.", 0.5f, 0.1f, 10.0f);

	appControlNamesCC.userCanAddControllables = true;
	appControlNamesCC.customUserCreateControllableFunc = std::bind(&OSModule::appControlCreateControllable, this, std::placeholders::_1);
	moduleParams.addChildControllableContainer(&appControlNamesCC);

	pingIPsCC.userCanAddControllables = true;
	pingIPsCC.customUserCreateControllableFunc = std::bind(&OSModule::pingIPsCreateControllable, this, std::placeholders::_1);
	moduleParams.addChildControllableContainer(&pingIPsCC);


	//Values
	osType = valuesCC.addEnumParameter("OS Type", "Type of OS");
	osType->addOption("Windows", OS_WIN)->addOption("MacOS", OS_MAC)->addOption("Linux", OS_LINUX);

#if JUCE_WINDOWS
	osType->setValueWithData(OS_WIN);
#elif JUCE_MAC
	osType->setValueWithData(OS_MAC);
#elif JUCE_LINUX
	osType->setValueWithData(OS_LINUX);
#endif

	osName = osInfoCC.addStringParameter("OS Name", "Name of the OS", SystemStats::getOperatingSystemName());
	terminateTrigger = valuesCC.addTrigger("Terminate", "This will be triggered when the program is about to terminate.");
	crashedTrigger = valuesCC.addTrigger("Crashed", "This will be triggered when the program has crashed, just before killing itself like a sad puppy.");

	osUpTime = osInfoCC.addFloatParameter("System Up Time", "Time since the system has been started, in seconds", 0, 0);
	osUpTime->defaultUI = FloatParameter::TIME;
	osCPUUsage = osInfoCC.addFloatParameter("CPU Usage", "Total CPU Usage from 0 to 1", 0, 0, 1);
	osMemoryUsage = osInfoCC.addFloatParameter("Memory Usage", "Total Memory Usage from 0 to 1", 0, 0, 1);

	processUpTime = osInfoCC.addFloatParameter("Process Up Time", "Time since the system this app has started, in seconds", 0, 0);
	processUpTime->defaultUI = FloatParameter::TIME;

	osInfoCC.addChildControllableContainer(&appControlStatusCC);
	valuesCC.addChildControllableContainer(&osInfoCC);

	ips = networkInfoCC.addStringParameter("IP", "IP that has been detected than most probable to be a LAN IP", NetworkHelpers::getLocalIP());
	networkInfoCC.addChildControllableContainer(&pingStatusCC);
	valuesCC.addChildControllableContainer(&networkInfoCC);

	Array<MACAddress> macList = MACAddress::getAllAddresses();
	String macs = "";
	for (int i = 0; i < macList.size(); i++) macs += (i > 0 ? "\n" : "") + macList[i].toString().replace("-", ":").toUpperCase();

	mac = networkInfoCC.addStringParameter("MAC", "Mac address of the IP", macs);
	mac->multiline = true;


	Array<WeakReference<Controllable>> cont = valuesCC.getAllControllables(true);
	for (auto& c : cont) c->isControllableFeedbackOnly = true;

	setupIOConfiguration(true, true);

	defManager->add(CommandDefinition::createDef(this, "Power", "Shutdown", &OSPowerCommand::create, CommandContext::ACTION)->addParam("type", OSPowerCommand::SHUTDOWN));
	defManager->add(CommandDefinition::createDef(this, "Power", "Reboot", &OSPowerCommand::create, CommandContext::ACTION)->addParam("type", OSPowerCommand::REBOOT));
	defManager->add(CommandDefinition::createDef(this, "Power", "Wake On Lan", &WakeOnLanCommand::create, CommandContext::ACTION));

	defManager->add(CommandDefinition::createDef(this, "Process", "Open File", &OSExecCommand::create, CommandContext::BOTH)->addParam("type", OSExecCommand::OPEN_FILE));
	defManager->add(CommandDefinition::createDef(this, "Process", "Launch App", &OSExecCommand::create, CommandContext::BOTH)->addParam("type", OSExecCommand::LAUNCH_APP));
	defManager->add(CommandDefinition::createDef(this, "Process", "Launch Command", &OSExecCommand::create, CommandContext::BOTH)->addParam("type", OSExecCommand::LAUNCH_COMMAND));
	defManager->add(CommandDefinition::createDef(this, "Process", "Launch Command File", &OSExecCommand::create, CommandContext::BOTH)->addParam("type", OSExecCommand::LAUNCH_COMMAND_FILE));
	defManager->add(CommandDefinition::createDef(this, "Process", "Kill App", &OSExecCommand::create, CommandContext::BOTH)->addParam("type", OSExecCommand::KILL_APP));
	defManager->add(CommandDefinition::createDef(this, "Window", "Set Window Parameters", &OSWindowCommand::create, CommandContext::BOTH));

	scriptObject.getDynamicObject()->setMethod(launchAppId, &OSModule::launchFileFromScript);
	scriptObject.getDynamicObject()->setMethod(launchCommandId, &OSModule::launchCommandFromScript);
	scriptObject.getDynamicObject()->setMethod(launchProcessId, &OSModule::launchProcessFromScript);
	scriptObject.getDynamicObject()->setMethod(getRunningProcessesId, &OSModule::getRunningProcessesFromScript);
	scriptObject.getDynamicObject()->setMethod(isProcessRunningId, &OSModule::isProcessRunningFromScript);

	startTimer(OS_IP_TIMER, 5000);
	startTimer(OS_APP_TIMER, 1000);

	osThread.startThread();
}

OSModule::~OSModule()
{
	stopTimer(OS_IP_TIMER);
	stopTimer(OS_APP_TIMER);
	stopThread(100);
	pingThread.stopThread(2000);
	osThread.stopThread(2000);
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
	var data = appControlStatusCC.getJSONData();
	appControlStatusCC.clear();

	for (auto& c : appControlNamesCC.controllables)
	{
		File f = ((FileParameter*)c)->getFile();
		BoolParameter* b = appControlStatusCC.addBoolParameter(f.existsAsFile() ? f.getFileName() : "[noapp]", "Status for this process", false);
		b->saveValueOnly = false;
	}

	appControlStatusCC.loadJSONData(data, false); //force reload styles

}

void OSModule::updatePingStatusValues()
{
	pingThread.stopThread(2000);
	var data = pingStatusCC.getJSONData();
	pingStatusCC.clear();

	for (auto& c : pingIPsCC.controllables)
	{
		String s = ((StringParameter*)c)->niceName;
		BoolParameter* b = pingStatusCC.addBoolParameter(s.isNotEmpty() ? s : "[noip]", "Status for this IP", false);
		b->saveValueOnly = false;
	}

	pingStatusCC.loadJSONData(data, false); //force reload styles
	for (auto& c : pingIPsCC.controllables)
	{
		if (BoolParameter* p = dynamic_cast<BoolParameter*>(c)) p->setValue(false);
	}

	if (enabled->boolValue() && pingIPsCC.controllables.size() > 0) pingThread.startThread();
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

void OSModule::childStructureChanged(ControllableContainer* c)
{
	Module::childStructureChanged(c);

	if (!isCurrentlyLoadingData && c == &moduleParams)
	{
		updatePingStatusValues(); //could be better filtered to avoid other structure than Ping IPs to trigger
	}
}

void OSModule::onContainerParameterChangedInternal(Parameter* p)
{
	Module::onContainerParameterChangedInternal(p);
	if (p == enabled)
	{
		if (!enabled->boolValue())  pingThread.stopThread(2000);
		else if (pingIPsCC.controllables.size() > 0) pingThread.startThread();
	}
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
	StringParameter* sp = new StringParameter("IP 1", "IP to ping", "");
	sp->userCanChangeName = true;
	sp->saveValueOnly = false;
	sp->isRemovableByUser = true;
	c->addParameter(sp);

	updatePingStatusValues();
}


bool OSModule::isProcessRunning(const String& processName)
{
	return getRunningProcesses().contains(processName);
}

StringArray OSModule::getRunningProcesses()
{
	StringArray result;

#if OS_SYSINFO_SUPPORT

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

#endif

	return result;
}

void OSModule::timerCallback(int timerID)
{
	inActivityTrigger->trigger();
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
		{
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
	for (auto& c : pingIPsCC.controllables)
	{
		c->userCanChangeName = true;
		c->saveValueOnly = false;
	}

	updatePingStatusValues();
}


void OSModule::PingThread::run()
{
#if PING_SUPPORT
	while (!threadShouldExit() && !moduleRef.wasObjectDeleted())
	{
		wait(osModule->pingInterval->intValue() * 1000);
		if (threadShouldExit()) return;

		Array<WeakReference<Parameter>> ipParams = osModule->pingIPsCC.getAllParameters();
		Array<WeakReference<Parameter>> statusParams = osModule->pingStatusCC.getAllParameters();

		if (ipParams.isEmpty()) return;
		//
		for (int i = 0; i < ipParams.size(); i++)
		{
			if (threadShouldExit() || moduleRef.wasObjectDeleted()) return;
			if (ipParams[i].wasObjectDeleted() || statusParams[i].wasObjectDeleted()) continue;
			String ip = ipParams[i]->stringValue();
			if (ip.isEmpty()) continue;
			//
			if (osModule->logOutgoingData->boolValue()) NLOG(osModule->niceName, "Pinging " + ipParams[i]->stringValue() + " ...");

			bool success = icmpPing(ip);

			if (osModule->logOutgoingData->boolValue())
			{
				if (success) NLOG(osModule->niceName, ipParams[i]->stringValue() << " is alive");
				else NLOGWARNING(osModule->niceName, ipParams[i]->stringValue() << " is dead");
			}

			if (!statusParams[i].wasObjectDeleted()) statusParams[i]->setValue(success);
		}
	}

	if (!moduleRef.wasObjectDeleted() && osModule->logOutgoingData->boolValue()) LOG("Stop pinging thread");
#endif
}

bool OSModule::PingThread::icmpPing(const String& host)
{
#if PING_SUPPORT
	int timeout_ms = osModule->pingTimeout->floatValue() * 1000;

#if JUCE_WINDOWS
	HANDLE hIcmpFile = IcmpCreateFile();
	if (hIcmpFile == INVALID_HANDLE_VALUE) {
		DBG("IcmpCreateFile failed");
		return false;
	}

	const int bufferSize = 32; // Size of the buffer in bytes
	BYTE sendBuffer[bufferSize];
	memset(sendBuffer, 'E', sizeof(sendBuffer)); // Filling buffer with arbitrary data

	int replySize = sizeof(ICMP_ECHO_REPLY) + bufferSize;
	LPVOID replyBuffer = malloc(replySize);
	if (replyBuffer == nullptr) {
		DBG("Memory allocation error");
		IcmpCloseHandle(hIcmpFile);
		return false;
	}

	DWORD reply = IcmpSendEcho(hIcmpFile, inet_addr(host.toStdString().c_str()), sendBuffer, sizeof(sendBuffer), nullptr, replyBuffer, replySize, timeout_ms);

	bool success = false;
	if (reply != 0) {
		success = true;
		//LOG("ICMP echo request successful");
		// Process the ICMP echo reply if needed
	}
	else {
		DBG("ICMP echo request failed: " << GetLastError());
	}

	free(replyBuffer);
	IcmpCloseHandle(hIcmpFile);

	return success;

#elif JUCE_LINUX || JUCE_MAC // UNIX
	int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sock < 0) {
		DBG("Socket creation error");
		return false;
	}

	struct timeval tv;
	tv.tv_sec = timeout_ms / 1000;
	tv.tv_usec = (timeout_ms % 1000) * 1000;
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&tv), sizeof(tv));

	char packet[1024];
	memset(packet, 0, sizeof(packet));

	struct icmphdr* icmp = reinterpret_cast<struct icmphdr*>(packet);
	icmp->type = ICMP_ECHO;
	icmp->code = 0;
	icmp->checksum = 0;
	icmp->un.echo.id = getpid();
	icmp->un.echo.sequence = 0;
	icmp->checksum = 0; // Calculate checksum

	struct sockaddr_in dest_addr;
	memset(&dest_addr, 0, sizeof(dest_addr));
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_addr.s_addr = inet_addr(host.toStdString().c_str());

	int bytes_sent = sendto(sock, packet, sizeof(packet), 0, reinterpret_cast<struct sockaddr*>(&dest_addr), sizeof(dest_addr));
	if (bytes_sent == -1 || bytes_sent != sizeof(packet)) {
		LOGWARNING("Error sending ICMP packet, you may need administrator/root privileges");
		close(sock);
		return false;
	}

	// Receive ICMP reply
	struct sockaddr_in from;
	socklen_t fromlen = sizeof(from);
	int bytes_received = recvfrom(sock, packet, sizeof(packet), 0, reinterpret_cast<struct sockaddr*>(&from), &fromlen);

	if (bytes_received > 0) {
		DBG("Received ICMP reply");
		// Process the ICMP reply if needed
	}
	else if (bytes_received == 0) {
		DBG("No data received");
	}
	else {
		if (errno == EAGAIN || errno == EWOULDBLOCK) {
			DBG("Timeout reached");
		}
		else {
			DBG("Receive error: " << strerror(errno));
		}
	}

	close(sock);
	return true; // Placeholder for successful ping
#else
	return false; //PLATFORM
#endif

#else// PING_SUPPORT
	return false;
#endif // PING_SUPPORT
}

void OSModule::OSThread::run()
{
	while (!threadShouldExit() && !moduleRef.wasObjectDeleted())
	{
		osModule->osUpTime->setValue((int)(Time::getMillisecondCounter() / 1000.0f));
		osModule->processUpTime->setValue((int)(osModule->osUpTime->floatValue() - OSModule::timeAtProcessStart));


		float usage = OSSystemInfo::getSystemCPUUsage();
		if (usage > 0) osModule->osCPUUsage->setValue(usage);
		osModule->osMemoryUsage->setValue(OSSystemInfo::getSystemMemoryUsageRatio());
		//osMemoryProcessUsage->setValue(maxMemProcess / memoryUsage[0].PhysicalTotalAvailable);

		if (osModule->logIncomingData->boolValue())
		{
			LOG("CPU Usage :" << (int)(osModule->osCPUUsage->floatValue() * 100) << "%, Memory Usage : " << (int)(osModule->osMemoryUsage->floatValue() * 100) << "%");
		}
	}
}
