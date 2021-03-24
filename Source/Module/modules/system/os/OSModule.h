/*
  ==============================================================================

    OSModule.h
    Created: 5 Jan 2018 3:41:34pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

class OSModule :
	public Module,
	public Timer,
	public Thread
{
public:
	OSModule();
	~OSModule();
	
	enum OSType { OS_WIN, OS_MAC, OS_LINUX };

	EnumParameter * osType;
	StringParameter * osName;
	StringParameter* ips;
	Trigger* terminateTrigger;

	//Script
	const Identifier launchAppId = "launchApp";
	const Identifier launchCommandId = "launchCommand";
	const Identifier launchProcessId = "launchProcess";

	//child process
	String commandToRun;
	std::unique_ptr<ChildProcess> childProcess;

	void updateIps();

	bool launchFile(File f, String args = "");
	void launchCommand(const String& command, bool silentMode);
	void launchChildProcess(const String& command);
	String launchChildProcessBlocking(const String& command);

	static var launchFileFromScript(const var::NativeFunctionArgs& args);
	static var launchCommandFromScript(const var::NativeFunctionArgs& args);
	static var launchProcessFromScript(const var::NativeFunctionArgs& args);

	void timerCallback() override;

	void run() override;

	virtual String getDefaultTypeString() const override { return "OS"; }
	static OSModule * create() { return new OSModule(); }
};