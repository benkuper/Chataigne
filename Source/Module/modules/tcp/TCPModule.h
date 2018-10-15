/*
  ==============================================================================

    TCPModule.h
    Created: 21 Oct 2017 5:04:54pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../common/streaming/NetworkStreamingModule.h"

class TCPModule :
	public NetworkStreamingModule,
	public Timer
{
public:
	TCPModule(const String &name = "TCP Client", int defaultRemotePort = 5001);
	virtual ~TCPModule();

	StreamingSocket sender;

	virtual void setupSender() override;
	virtual void initThread() override;
	virtual void clearThread() override;

	virtual bool checkReceiverIsReady() override;
	virtual bool isReadyToSend() override;

	virtual void sendMessageInternal(const String &message) override;
	virtual void sendBytesInternal(Array<uint8> data) override;

	virtual Array<uint8> readBytes() override;
	
	virtual void clearInternal() override;

	static TCPModule * create() { return new TCPModule(); }
	virtual String getDefaultTypeString() const override { return "TCP Client"; }

	// Inherited via Timer
	virtual void timerCallback() override;
};