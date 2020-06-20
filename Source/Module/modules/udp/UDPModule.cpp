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
	scriptObject.setMethod("sendTo", &UDPModule::sendBytesToFromScript);
	scriptObject.setMethod("sendMessageTo", &UDPModule::sendMessageToFromScript);

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
		receiver->setEnablePortReuse(false);

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
	sender.reset(new DatagramSocket(true));
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

void UDPModule::sendMessageInternal(const String & message, var params)
{
	String targetHost = params.getProperty("ip", useLocal->boolValue() ? "127.0.0.1" : remoteHost->stringValue());
	int port = params.getProperty("port", remotePort->intValue());
	sender->write(targetHost, port, message.getCharPointer(), message.length());
}

void UDPModule::sendBytesInternal(Array<uint8> data, var params)
{
	String targetHost = params.getProperty("ip", useLocal->boolValue() ? "127.0.0.1" : remoteHost->stringValue());
	int port = params.getProperty("port", remotePort->intValue());
	sender->write(targetHost, port, data.getRawDataPointer(), data.size());
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

var UDPModule::sendMessageToFromScript(const var::NativeFunctionArgs& a)
{
	StreamingModule* m = getObjectFromJS<StreamingModule>(a);
	if (checkNumArgs(m->niceName, a, 3)) return false;

	var params(new DynamicObject());
	params.getDynamicObject()->setProperty("ip", a.arguments[0].toString());
	params.getDynamicObject()->setProperty("port", (int)a.arguments[1]);

	m->sendMessage(a.arguments[2].toString(), params);

	return var();
}

var UDPModule::sendBytesToFromScript(const var::NativeFunctionArgs& a)
{
	StreamingModule* m = getObjectFromJS<StreamingModule>(a);
	if (checkNumArgs(m->niceName, a, 3)) return false;
	Array<uint8> data;
	for (int i = 2; i < a.numArguments; ++i)
	{
		if (a.arguments[i].isArray())
		{
			Array<var>* aa = a.arguments[i].getArray();
			for (auto& vaa : *aa) data.add((uint8)(int)vaa);
		}
		else if (a.arguments[i].isInt() || a.arguments[i].isDouble() || a.arguments[i].isInt64() || a.arguments[i].isBool())
		{
			data.add((uint8)(int)a.arguments[i]);
		}
	}

	var params(new DynamicObject());
	params.getDynamicObject()->setProperty("ip", a.arguments[0].toString());
	params.getDynamicObject()->setProperty("port", (int)a.arguments[1]);
	m->sendBytes(data,params);
	return var();

}
