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
	TCPModule(const String &name = "TCP", int defaultRemotePort = 50000);
	~TCPModule();

	enum Mode { LINES, DATA255, RAW, COBS };
	EnumParameter * modeParam;

	//CLIENT
	BoolParameter * useLocal;
	StringParameter * remoteHost;
	IntParameter * remotePort;
	BoolParameter * isConnected;
	ScopedPointer<StreamingSocket> sender;

	Trigger * reconnectRemote;

	//Script
	const Identifier tcpEventId = "tcpEvent";
	const Identifier sendTCPId = "send";
	const Identifier writeTCPId = "write";

	//SEND
	void setupSender();
	void sendStringPacket(const String &s);
	void sendRawData(Array<uint8> data);

	virtual void processRawData(Array<uint8> data);
	virtual void processMessage(const String & msg);
	virtual void processMessageInternal(const String &) {}

	virtual void onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c) override;

	static TCPModule * create() { return new TCPModule(); }
	virtual String getDefaultTypeString() const override { return "TCP"; }


	//Script
	static var sendMessageFromScript(const var::NativeFunctionArgs &args);
	static var writeDataFromScript(const var::NativeFunctionArgs &args);
	 
	// Inherited via Thread
	virtual void run() override;
};