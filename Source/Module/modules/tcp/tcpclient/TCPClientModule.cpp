/*
  ==============================================================================

	TCPClientModule.cpp
	Created: 21 Oct 2017 5:04:54pm
	Author:  Ben

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

TCPClientModule::TCPClientModule(const String& name, int defaultRemotePort, bool autoConnect) :
	NetworkStreamingModule(name, false, true, 0, defaultRemotePort),
	autoConnect(autoConnect),
	autoReconnect(true)
{
	connectionFeedbackRef = senderIsConnected;

	setupIOConfiguration(true, true);

	if (!Engine::mainEngine->isLoadingFile)
	{
		setupSender();
	}

}

TCPClientModule::~TCPClientModule()
{
}

void TCPClientModule::setupSender()
{
	clearThread();
	clearInternal();

	sendCC->clearWarning();

	if (!enabled->boolValue() || isCurrentlyLoadingData ||
		sendCC == nullptr || !sendCC->enabled->boolValue() ||
		(Engine::mainEngine != nullptr && Engine::mainEngine->isClearing))
	{
		//if(!sendCC->enabled->boolValue()) sendCC->clearWarning();
		//stopThread(1000);
		return;
	}

	if (autoConnect) startThread();
}

void TCPClientModule::initThread()
{
	if (!enabled->boolValue()) return;

	if (senderIsConnected->boolValue() || sender.isConnected())
	{
		sender.close();
		senderIsConnected->setValue(false);
	}

	while (!senderIsConnected->boolValue() && !threadShouldExit())
	{
		bool result = connect();
		if (!result)
		{
			if (!autoReconnect)
			{
				signalThreadShouldExit();
				break;
			}
			else wait(1000);
		}
	}
}

void TCPClientModule::clearThread()
{
	NetworkStreamingModule::clearThread();
	if (sender.isConnected())
	{
		sender.close();
		senderIsConnected->setValue(false);
	}
}

bool TCPClientModule::checkReceiverIsReady()
{
	if (!senderIsConnected->boolValue()) return false;
	int result = sender.waitUntilReady(true, 100);

	if (result == -1)
	{
		senderIsConnected->setValue(false);
		return false;
	}

	return result == 1;
}

bool TCPClientModule::isReadyToSend()
{
	return !autoConnect || senderIsConnected->boolValue();
}

void TCPClientModule::sendMessageInternal(const String& message, var)
{
	int numBytes = sender.write(message.getCharPointer(), message.length());
	if (numBytes == -1)
	{
		NLOGERROR(niceName, "Error sending message");
		senderIsConnected->setValue(false);
	}
}

void TCPClientModule::sendBytesInternal(Array<uint8> data, var)
{
	int numBytes = sender.write(data.getRawDataPointer(), data.size());
	if (numBytes == -1)
	{
		NLOGERROR(niceName, "Error sending data");
		senderIsConnected->setValue(false);
	}
}

Array<uint8> TCPClientModule::readBytes()
{
	uint8 bytes[2048];
	int numRead = sender.read(bytes, 2048, false);

	if (numRead == 0)
	{
		NLOGWARNING(niceName, "Connection to TCP Server seems lost, disconnecting");
		senderIsConnected->setValue(false);
	}

	return Array<uint8>(bytes, numRead);
}

void TCPClientModule::clearInternal()
{
	if (sender.isConnected())
	{
		sender.close();
		senderIsConnected->setValue(false);
	}
}

void TCPClientModule::runInternal()
{
	if (!sender.isConnected() || !senderIsConnected->boolValue())
	{
		senderIsConnected->setValue(false);
		wait(1000);
		if (threadShouldExit()) return;
		if (autoReconnect) initThread();
	}
}

bool TCPClientModule::connect()
{
	String targetHost = useLocal->boolValue() ? "127.0.0.1" : remoteHost->stringValue();
	sender.bindToPort(0, networkInterface->getIP());
	bool result = sender.connect(targetHost, remotePort->intValue(), 200);
	if (result && autoConnect) NLOG(niceName, "Sender bound to port " << sender.getBoundPort());
	senderIsConnected->setValue(result);

	if (result)
	{
		NLOG(niceName, "Client is connected to " << remoteHost->stringValue() << ":" << remotePort->intValue());
		sendCC->clearWarning();
	}
	else
	{
		String s = "Could not connect to " + remoteHost->stringValue() + ":" + remotePort->stringValue();
		if (sendCC->getWarningMessage().isEmpty()) NLOGERROR(niceName, s);
		sendCC->setWarningMessage(s);
	}

	return result;
}
