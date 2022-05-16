/*
  ==============================================================================

    UDPModule.h
    Created: 2 Jan 2018 11:54:51am
    Author:  Ben

  ==============================================================================
*/

#pragma once


class UDPModule :
	public NetworkStreamingModule
{
public:
	UDPModule(const String &name = "UDP", bool canHaveInput = true, bool canHaveOutput = true, int defaultLocalPort= 10000, int defaultRemotePort = 10001);
	virtual ~UDPModule();

	BoolParameter* multicastMode;

	std::unique_ptr<DatagramSocket> receiver;
	std::unique_ptr<DatagramSocket> sender;
	DatagramSocket* proxySender; //if receiver is enabled, then proxy sender is receiver. Allows for feedback
	BoolParameter* listenToOutputFeedback;

	virtual void setupReceiver() override;
	virtual void setupSender() override;

	virtual bool checkReceiverIsReady() override;
	virtual bool isReadyToSend() override;

	virtual void sendMessageInternal(const String &message, var params) override;
	virtual void sendBytesInternal(Array<uint8> data, var params) override;

	virtual Array<uint8> readBytes() override;

	virtual void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;

	virtual void clearInternal() override;


	static var sendMessageToFromScript(const var::NativeFunctionArgs& a);
	static var sendBytesToFromScript(const var::NativeFunctionArgs& a);

	static UDPModule * create() { return new UDPModule(); } 
	virtual String getDefaultTypeString() const override { return "UDP"; }
};