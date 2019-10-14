/*
  ==============================================================================

    UDPModule.cpp
    Created: 2 Jan 2018 11:54:51am
    Author:  Ben

  ==============================================================================
*/

#include "UDPModule.h"

UDPModule::UDPModule(const String & name, bool canHaveInput, bool canHaveOutput, int defaultLocalPort, int defaultRemotePort) :
	NetworkStreamingModule(name, canHaveInput,canHaveOutput,defaultLocalPort,defaultRemotePort)
{
	if (senderIsConnected != nullptr) senderIsConnected->hideInOutliner = true; //no need because UDP doesn't check remote client existance
	if(!Engine::mainEngine->isLoadingFile) setupReceiver();
	setupSender();
}

UDPModule::~UDPModule()
{
}

void UDPModule::setupReceiver()
{
	clearThread();
	clearInternal();

	if (receiveCC == nullptr) return;
	if (!receiveCC->enabled->boolValue())
	{
		localPort->clearWarning();
		return;
	}

	receiver.reset(new DatagramSocket());
	receiver->bindToPort(localPort->intValue());
	receiverIsBound->setValue(receiver->getBoundPort() != -1);
	if (receiverIsBound->boolValue())
	{
		receiver->setEnablePortReuse(true);

		NLOG(niceName, "UDP Receiver bound to port " << localPort->intValue());
		localPort->clearWarning();
		startThread();
	}
	else
	{
		NLOGERROR(niceName, "UDP Receiver bound to port " << localPort->intValue());
		localPort->setWarningMessage("Could not bind to port " + String(localPort->intValue()));
	}

	Array<IPAddress> ad;
	IPAddress::findAllAddresses(ad);
	Array<String> ips;
	for (auto &a : ad) ips.add(a.toString());
	ips.sort();
	String s = "Local IPs:";
	for (auto &ip : ips) s += String("\n > ") + ip;
}

void UDPModule::setupSender()
{
	sender = nullptr;
	if (sendCC == nullptr) return;
	if (!sendCC->enabled->boolValue()) return;
	sender.reset(new DatagramSocket());
	senderIsConnected->setValue(true);
}

bool UDPModule::checkReceiverIsReady()
{
	if (receiver == nullptr) return false;
	if (receiver->getBoundPort() == -1) return false;
	return receiver->waitUntilReady(true, 300) == 1;
}

bool UDPModule::isReadyToSend()
{
	if (sender == nullptr) return false;
	return sender->waitUntilReady(false, 300) == 1;
}

void UDPModule::sendMessageInternal(const String & message)
{
	String targetHost = useLocal->boolValue() ? "127.0.0.1" : remoteHost->stringValue();
	sender->write(targetHost, remotePort->intValue(),message.getCharPointer(), message.length());
}

void UDPModule::sendBytesInternal(Array<uint8> data)
{
	String targetHost = useLocal->boolValue() ? "127.0.0.1" : remoteHost->stringValue();
	sender->write(targetHost, remotePort->intValue(), data.getRawDataPointer(), data.size());
}

Array<uint8> UDPModule::readBytes()
{
	Array<uint8> result;
	uint8 data[255];
	while (true)
	{
		int numBytes = receiver->read(data, 255, false);

		if (numBytes == -1)
		{
			LOGERROR("Error receiving UDP data");
			return result;
		}

		if (numBytes == 0) break;

		result.addArray((const uint8 *)data, numBytes);
	}
	
	return result;

}

void UDPModule::clearInternal()
{
	if (receiver != nullptr) receiver->shutdown();
	receiver = nullptr;
}
