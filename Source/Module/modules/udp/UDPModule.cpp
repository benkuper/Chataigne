/*
  ==============================================================================

	UDPModule.cpp
	Created: 2 Jan 2018 11:54:51am
	Author:  Ben

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

UDPModule::UDPModule(const String& name, bool canHaveInput, bool canHaveOutput, int defaultLocalPort, int defaultRemotePort) :
	NetworkStreamingModule(name, canHaveInput, canHaveOutput, defaultLocalPort, defaultRemotePort)
{
	multicastMode = moduleParams.addBoolParameter("Multicast Mode", "If check, instead of binding and connecting, it will try to join a multicast network.", false);

	scriptObject.getDynamicObject()->setMethod("sendTo", &UDPModule::sendMessageToFromScript);
	scriptObject.getDynamicObject()->setMethod("sendBytesTo", &UDPModule::sendBytesToFromScript);

	listenToOutputFeedback = sendCC->addBoolParameter("Listen to Feedback", "If checked, this will listen to the (randomly set) bound port of this sender. This is useful when some softwares automatically detect incoming host and port to send back messages.", false);

	if (!Engine::mainEngine->isLoadingFile) setupReceiver();
	setupSender();
}

UDPModule::~UDPModule()
{
}

void UDPModule::setupReceiver()
{
	clearThread();
	clearInternal();

	receiverIsBound->setValue(false);

	if (!enabled->boolValue()) return;
	if (receiveCC == nullptr) return;
	if (!receiveCC->enabled->boolValue())
	{
		localPort->clearWarning();
		return;
	}

	receiver.reset(new DatagramSocket());

	bool syncSenderPort = sender != nullptr && sendCC != nullptr && sendCC->enabled->boolValue() && listenToOutputFeedback->boolValue();
	receiver->setEnablePortReuse(syncSenderPort);

	receiver->bindToPort(localPort->intValue());
	receiverIsBound->setValue(receiver->getBoundPort() != -1);

	if (receiverIsBound->boolValue())
	{
		NLOG(niceName, "UDP Receiver bound to port " << localPort->intValue());
		localPort->clearWarning();
		if (syncSenderPort) setupSender();

		if (multicastMode->boolValue())
		{
			NLOG(niceName, "UDP Multicast mode, using Ouput's Remote Host to join multicast : " << remoteHost->stringValue());
			receiver->joinMulticast(remoteHost->stringValue());
		}

		startThread();
	}
	else
	{
		NLOGERROR(niceName, "Error binding to port " << localPort->intValue());
		localPort->setWarningMessage("Could not bind to port " + String(localPort->intValue()));
	}

	Array<IPAddress> ad;
	IPAddress::findAllAddresses(ad);
	Array<String> ips;
	for (auto& a : ad) ips.add(a.toString());
	ips.sort();
	String s = "Local IPs:";
	for (auto& ip : ips) s += String("\n > ") + ip;
}

void UDPModule::setupSender()
{
	sender.reset();
	proxySender = nullptr;

	clearWarning("Multicast");

	if (!enabled->boolValue()) return;
	if (sendCC == nullptr) return;
	if (!sendCC->enabled->boolValue()) return;

	sender.reset(new DatagramSocket(!multicastMode->boolValue()));

	bool syncReceiverPort = receiver != nullptr && receiveCC != nullptr && receiveCC->enabled->boolValue() && receiverIsBound->boolValue() && listenToOutputFeedback->boolValue();

	if (multicastMode->boolValue())
	{
		sender->setEnablePortReuse(true);
		bool portBound = sender->bindToPort(remotePort->intValue());
		if (!portBound)
		{
			NLOGERROR(niceName, "Could not bind multicast port " << remotePort->stringValue());
			setWarningMessage("Could not bind multicast port " + remotePort->stringValue(), "Multicast");
		}
		else
		{
			bool multicastResult = sender->joinMulticast(remoteHost->stringValue());
			if (!multicastResult)
			{
				NLOGERROR(niceName, "Could not join multicast on address " << remoteHost->stringValue());
				setWarningMessage("Could not join multicast", "Multicast");
			}
			else
			{
				clearWarning("Multicast");
				if (syncReceiverPort) sender->setMulticastLoopbackEnabled(true);
			}
		}
	}
	else if (syncReceiverPort)
	{
		sender->setEnablePortReuse(true);
		sender->bindToPort(receiver->getBoundPort());
	}
	else
	{
		sender->bindToPort(0);
	}



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

void UDPModule::sendMessageInternal(const String& message, var params)
{
	String targetHost = params.getProperty("ip", useLocal->boolValue() ? "127.0.0.1" : remoteHost->stringValue());
	int port = params.getProperty("port", remotePort->intValue());
	if (sender != nullptr) sender->write(targetHost, port, message.getCharPointer(), message.length());
}

void UDPModule::sendBytesInternal(Array<uint8> data, var params)
{
	String targetHost = params.getProperty("ip", useLocal->boolValue() ? "127.0.0.1" : remoteHost->stringValue());
	int port = params.getProperty("port", remotePort->intValue());
	if (sender != nullptr) sender->write(targetHost, port, data.getRawDataPointer(), data.size());
}

Array<uint8> UDPModule::readBytes()
{
	Array<uint8> result;

	while (true)
	{
		String senderAddress = "";
		int senderPort = 0;
		int numBytes = receiver->read(data, UDP_MAX_PACKET_SIZE, false, senderAddress, senderPort);

		if (numBytes == -1)
		{
			LOGERROR("Error receiving UDP data");
			return result;
		}

		if (numBytes == 0) break;

		result.addArray((const uint8*)data, numBytes);
	}

	return result;

}

void UDPModule::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	NetworkStreamingModule::onControllableFeedbackUpdateInternal(cc, c);
	if (c == multicastMode)
	{
		setupReceiver();
		setupSender();
	}
}

void UDPModule::clearInternal()
{
	if (receiver != nullptr) receiver->shutdown();
	if (proxySender == receiver.get()) proxySender = nullptr;
	receiver.reset();
}

var UDPModule::sendMessageToFromScript(const var::NativeFunctionArgs& a)
{
	StreamingModule* m = getObjectFromJS<StreamingModule>(a);
	if (!checkNumArgs(m->niceName, a, 3)) return false;

	var params(new DynamicObject());
	params.getDynamicObject()->setProperty("ip", a.arguments[0].toString());
	params.getDynamicObject()->setProperty("port", (int)a.arguments[1]);

	m->sendMessage(a.arguments[2].toString(), params);

	return var();
}

var UDPModule::sendBytesToFromScript(const var::NativeFunctionArgs& a)
{
	StreamingModule* m = getObjectFromJS<StreamingModule>(a);
	if (!checkNumArgs(m->niceName, a, 3)) return false;
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
	m->sendBytes(data, params);
	return var();

}
