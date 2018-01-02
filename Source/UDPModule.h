/*
  ==============================================================================

    UDPModule.h
    Created: 2 Jan 2018 11:54:51am
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "Module.h"

class UDPModule :
	public Module,
	public Thread
{
public:
	UDPModule(const String &name ="UDP", int defaultLocalPort = 12000, int defaultRemotePort = 9000, bool canHaveInput = true, bool canHaveOutput = true);
	~UDPModule();

	enum Mode { LINES, DATA255, RAW, COBS };
	EnumParameter * modeParam;

	//RECEIVE
	IntParameter * localPort;
	BoolParameter * isConnected;
	ScopedPointer<DatagramSocket> receiver;

	//SEND
	BoolParameter * useLocal;
	StringParameter * remoteHost;
	IntParameter * remotePort;
	ScopedPointer<DatagramSocket> sender;

	ScopedPointer<EnablingControllableContainer> receiveCC;
	ScopedPointer<EnablingControllableContainer> sendCC;

	//Script
	const Identifier udpEventId = "udpEvent";
	const Identifier sendUDPId = "send";

	//RECEIVE
	virtual void setupReceiver();

	void processMessage(const String & msg);
	virtual void processMessageInternal(const String &) {}

	//SEND
	virtual void setupSender();
	void sendMessage(const String &msg);

	//Script
	static var sendMessageFromScript(const var::NativeFunctionArgs &args);

	//save / load
	virtual var getJSONData() override;
	virtual void loadJSONDataInternal(var data) override;

	virtual void onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c) override;

	static UDPModule * create() { return new UDPModule(); }
	virtual String getDefaultTypeString() const override { return "UDP"; }


	// Inherited via Thread
	virtual void run() override;
};
