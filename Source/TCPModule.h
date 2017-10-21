/*
  ==============================================================================

    TCPModule.h
    Created: 21 Oct 2017 5:04:54pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "Module.h"

class TCPModule :
	public Module,
	public Thread
{
public:
	TCPModule(const String &name = "TCP Module", int defaultLocalPort = 40000, int defaultRemotePort = 50000);
	~TCPModule();

	

	//RECEIVE
	IntParameter * localPort;
	BoolParameter * isBound;
	StreamingSocket receiver;

	//SEND
	BoolParameter * useLocal;
	StringParameter * remoteHost;
	IntParameter * remotePort;
	BoolParameter * isConnected;
	StreamingSocket sender;

	Trigger * reconnectRemote;

	//RECEIVE
	void setupReceiver();

	//SEND
	void setupSender();
	void sendStringPacket(const String &s);

	virtual void onContainerParameterChangedInternal(Parameter * p) override;
	virtual void onContainerTriggerTriggered(Trigger * t) override;


	static TCPModule * create() { return new TCPModule(); }
	virtual String getDefaultTypeString() const override { return "TCP"; }

	InspectableEditor * getEditor(bool isRoot) override;

	//Script
	/*
	static var sendPacketFromScript(const var::NativeFunctionArgs &args);


	static OSCArgument varToPacket(const var &v);
	static var packetToVar(const OSCArgument &a);
	*/


	// Inherited via Thread
	virtual void run() override;

};