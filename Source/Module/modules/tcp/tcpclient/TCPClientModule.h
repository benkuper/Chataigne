/*
  ==============================================================================

    TCPClientModule.h
    Created: 21 Oct 2017 5:04:54pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

class TCPClientModule :
	public NetworkStreamingModule
{
public:
	TCPClientModule(const String &name = "TCP Client", int defaultRemotePort = 5001, bool autoConnect = true);
	virtual ~TCPClientModule();

	StreamingSocket sender;

	bool autoConnect;
	bool autoReconnect;

	virtual void setupSender() override;
	virtual void initThread() override;
	virtual void clearThread() override;

	virtual bool checkReceiverIsReady() override;
	virtual bool isReadyToSend() override;

	virtual void sendMessageInternal(const String& message, var) override;
	virtual void sendBytesInternal(Array<uint8> data, var) override;

	virtual Array<uint8> readBytes() override;
	
	virtual void clearInternal() override;

	virtual void runInternal() override;

	bool connect();

	static TCPClientModule * create() { return new TCPClientModule(); }
	virtual String getDefaultTypeString() const override { return "TCP Client"; }

};