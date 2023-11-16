/*
  ==============================================================================

	OSModule.h
	Created: 5 Jan 2018 3:41:34pm
	Author:  Ben

  ==============================================================================
*/

#pragma once

#define OS_IP_TIMER 1
#define OS_APP_TIMER 2

class OSModule :
	public Module,
	public MultiTimer,
	public Thread
{
public:
	OSModule();
	~OSModule();

	enum OSType { OS_WIN, OS_MAC, OS_LINUX };

	Trigger* listIPs;
	IntParameter* pingInterval; // in seconds
	FloatParameter* pingTimeout;

	Trigger* terminateTrigger;
	Trigger* crashedTrigger;

	ControllableContainer osInfoCC;
	EnumParameter* osType;
	StringParameter* osName;
	FloatParameter* osCPUUsage;
	FloatParameter* osMemoryUsage;
	FloatParameter* osUpTime;
	FloatParameter* processUpTime;

	ControllableContainer networkInfoCC;
	StringParameter* ips;
	StringParameter* mac;

	ControllableContainer appControlNamesCC;
	ControllableContainer appControlStatusCC;

	ControllableContainer pingIPsCC;
	ControllableContainer pingStatusCC;

	static float timeAtProcessStart;

	//Script
	const Identifier launchAppId = "launchApp";
	const Identifier launchCommandId = "launchCommand";
	const Identifier launchProcessId = "launchProcess";
	const Identifier getRunningProcessesId = "getRunningProcesses";
	const Identifier isProcessRunningId = "isProcessRunning";

	//child process
	Array<String, CriticalSection> commandsToRun;

	class OSThread :
		public Thread
	{
	public:
		OSThread(OSModule* m) : Thread("OS"), osModule(m), moduleRef(m) {}
		~OSThread() {}

		OSModule* osModule;
		WeakReference<Inspectable> moduleRef;
		void run();
	};

	OSThread osThread;

	class PingThread :
		public Thread
	{
	public:
		PingThread(OSModule* m) : Thread("Ping"), osModule(m), moduleRef(m) {}
		~PingThread() {}

		OSModule* osModule;
		WeakReference<Inspectable> moduleRef;

		void run() override;
		bool icmpPing(const String& host);
	};

	PingThread pingThread;

	void updateIps();

	bool launchFile(File f, String args = "");
	void launchCommand(const String& command, bool silentMode);
	void launchChildProcess(const String& command);
	String launchChildProcessBlocking(const String& command);
	void killProcess(const String& processName, bool hardKillMode);

	void checkAppControl();
	void updateAppControlValues();
	void updatePingStatusValues();

	void appControlCreateControllable(ControllableContainer* c);
	void pingIPsCreateControllable(ControllableContainer* c);

	void childStructureChanged(ControllableContainer* c) override;
	void onContainerParameterChangedInternal(Parameter* p) override;
	void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;

	static var launchFileFromScript(const var::NativeFunctionArgs& args);
	static var launchCommandFromScript(const var::NativeFunctionArgs& args);
	static var launchProcessFromScript(const var::NativeFunctionArgs& args);
	static var isProcessRunningFromScript(const var::NativeFunctionArgs& args);
	static var getRunningProcessesFromScript(const var::NativeFunctionArgs& args);

	bool isProcessRunning(const String& processName);
	StringArray getRunningProcesses();

	void timerCallback(int timerID) override;
	void run() override;

	void afterLoadJSONDataInternal() override;

	virtual String getDefaultTypeString() const override { return "OS"; }
	static OSModule* create() { return new OSModule(); }
};
