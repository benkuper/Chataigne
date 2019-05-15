/*
  ==============================================================================

    UDPModule.h
    Created: 2 Jan 2018 11:54:51am
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../common/streaming/NetworkStreamingModule.h"

class UDPModule :
	public NetworkStreamingModule
{
public:
	UDPModule(const String &name = "UDP", bool canHaveInput = true, bool canHaveOutput = true, int defaultLocalPort= 10000, int defaultRemotePort = 10001);
	virtual ~UDPModule();

	std::unique_ptr<DatagramSocket> receiver;
	std::unique_ptr<DatagramSocket> sender;

	virtual void setupReceiver() override;
	virtual void setupSender() override;

	virtual bool checkReceiverIsReady() override;
	virtual bool isReadyToSend() override;

	virtual void sendMessageInternal(const String &message) override;
	virtual void sendBytesInternal(Array<uint8> data) override;

	virtual Array<uint8> readBytes() override;

	virtual void clearInternal() override;

	static UDPModule * create() { return new UDPModule(); } 
	virtual String getDefaultTypeString() const override { return "UDP"; }
};