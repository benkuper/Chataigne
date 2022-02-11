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
	
	EnumParameter * osType;
	StringParameter * osName;
	StringParameter* ips;
	Trigger* terminateTrigger;
	Trigger* crashedTrigger;

	ControllableContainer appControlNamesCC;
	ControllableContainer appControlStatusCC;

	//Script
	const Identifier launchAppId = "launchApp";
	const Identifier launchCommandId = "launchCommand";
	const Identifier launchProcessId = "launchProcess";
	const Identifier getRunningProcessesId = "getRunningProcesses";
	const Identifier isProcessRunningId = "isProcessRunning";

	//child process
	String commandToRun;
	std::unique_ptr<ChildProcess> childProcess;


	void updateIps();

	bool launchFile(File f, String args = "");
	void launchCommand(const String& command, bool silentMode);
	void launchChildProcess(const String& command);
	String launchChildProcessBlocking(const String& command);
	void killProcess(const String& processName, bool hardKillMode);

	void checkAppControl();
	void updateAppControlValues();

	void appControlCreateControllable(ControllableContainer* c);

	void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;

	static var launchFileFromScript(const var::NativeFunctionArgs& args);
	static var launchCommandFromScript(const var::NativeFunctionArgs& args);
	static var launchProcessFromScript(const var::NativeFunctionArgs& args);
	static var isProcessRunningFromScript(const var::NativeFunctionArgs& args);
	static var getRunningProcessesFromScript(const var::NativeFunctionArgs& args);

	bool isProcessRunning(const String &processName);
	StringArray getRunningProcesses();

	void timerCallback(int timerID) override;
	void run() override;


	void afterLoadJSONDataInternal() override;

	virtual String getDefaultTypeString() const override { return "OS"; }
	static OSModule * create() { return new OSModule(); }
};