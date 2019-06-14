/*
  ==============================================================================

    TCPModule.cpp
    Created: 21 Oct 2017 5:04:54pm
    Author:  Ben

  ==============================================================================
*/

#include "TCPModule.h"

TCPModule::TCPModule(const String & name, int defaultRemotePort) :
	NetworkStreamingModule(name,false, true, 0, defaultRemotePort)
{
	connectionFeedbackRef = senderIsConnected;

	setupIOConfiguration(true, true);
	autoAdd->setValue(false);
	setupSender();
	startTimerHz(1);

}

TCPModule::~TCPModule()
{
}

void TCPModule::setupSender()
{
	clearThread();
	clearInternal();
	
	if (sendCC == nullptr) return;
	if (!sendCC->enabled->boolValue()) return;
	if (Engine::mainEngine != nullptr && Engine::mainEngine->isClearing) return;

	startThread();
}

void TCPModule::initThread()
{
	//if (sender.isConnected()) sender.close();

	String targetHost = useLocal->boolValue() ? "127.0.0.1" : remoteHost->stringValue();
	bool result = sender.connect(targetHost, remotePort->intValue(), 200);
	if(result) NLOG(niceName, "Sender bound to port " << sender.getBoundPort());
	senderIsConnected->setValue(result);
}

void TCPModule::clearThread()
{
	NetworkStreamingModule::clearThread();
	if (sender.isConnected()) sender.close();
}

bool TCPModule::checkReceiverIsReady()
{
	if (!senderIsConnected->boolValue()) return false;
	int result = sender.waitUntilReady(true, 100);
	
	if(result == -1)
	{
		senderIsConnected->setValue(false);
		return false;
	}

	return result == 1;
}

bool TCPModule::isReadyToSend()
{
	return senderIsConnected->boolValue();
}

void TCPModule::sendMessageInternal(const String & message)
{
	int numBytes = sender.write(message.getCharPointer(), message.length());
	if (numBytes == -1)
	{
		NLOGERROR(niceName, "Error sending message");
		senderIsConnected->setValue(false);
	}
}

void TCPModule::sendBytesInternal(Array<uint8> data)
{
	int numBytes = sender.write(data.getRawDataPointer(), data.size());
	if (numBytes == -1)
	{
		NLOGERROR(niceName, "Error sending data");
		senderIsConnected->setValue(false);
	}
}

Array<uint8> TCPModule::readBytes()
{
	uint8 bytes[255];
	int numRead = sender.read(bytes, 255, false);
	DBG("Num read : " << numRead);
	return Array<uint8>(bytes, numRead);
}

void TCPModule::clearInternal()
{
	if (sender.isConnected()) sender.close();
}

void TCPModule::timerCallback()
{
	if (!sender.isConnected()) senderIsConnected->setValue(false);
	if(!senderIsConnected->boolValue()) setupSender();
}
