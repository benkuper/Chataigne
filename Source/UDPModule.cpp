/*
  ==============================================================================

    UDPModule.cpp
    Created: 2 Jan 2018 11:54:51am
    Author:  Ben

  ==============================================================================
*/

#include "UDPModule.h"
#include "SendUDPStringCommand.h"

UDPModule::UDPModule(const String & name, int defaultLocalPort, int defaultRemotePort, bool canHaveInput, bool canHaveOutput) :
	Module(name),
	Thread("udpReceive")
{

	setupIOConfiguration(canHaveInput, canHaveOutput);
	canHandleRouteValues = canHaveOutput;


	//Receive
	if (canHaveInput)
	{
		receiveCC = new EnablingControllableContainer("UDP Input");
		moduleParams.addChildControllableContainer(receiveCC);

		localPort = receiveCC->addIntParameter("Local Port", "Local Port to bind to receive UDP Messages", defaultLocalPort, 1024, 65535);
		localPort->hideInOutliner = true;
		localPort->isTargettable = false;

		modeParam = receiveCC->addEnumParameter("Mode", "Protocol for treating the incoming data");
		modeParam->addOption("Lines", LINES)->addOption("Raw", RAW)->addOption("Data255", DATA255)->addOption("COBS", COBS);

		setupReceiver();
	}

	//Send
	if (canHaveOutput)
	{
		sendCC = new EnablingControllableContainer("UDP Output");
		moduleParams.addChildControllableContainer(sendCC);

		useLocal = sendCC->addBoolParameter("Local", "Send to Local IP (127.0.0.1). Allow to quickly switch between local and remote IP.", true);
		remoteHost = sendCC->addStringParameter("Remote Host", "Remote Host to send to.", "127.0.0.1");
		remoteHost->setEnabled(!useLocal->boolValue());
		remotePort = sendCC->addIntParameter("Remote port", "Port on which the remote host is listening to", defaultRemotePort, 1024, 65535);

		setupSender();
	}

	defManager.add(CommandDefinition::createDef(this, "", "Send string", &SendUDPStringCommand::create, CommandContext::ACTION));

	//Script
	scriptObject.setMethod(sendUDPId, UDPModule::sendMessageFromScript);
}

UDPModule::~UDPModule()
{
	signalThreadShouldExit();
	while (isThreadRunning());
	if (receiver != nullptr) receiver->shutdown();
}

void UDPModule::setupReceiver()
{
	if (receiveCC == nullptr) return;

	signalThreadShouldExit();
	while (isThreadRunning());
	receiver = nullptr;
	
	if (!receiveCC->enabled->boolValue()) return;

	receiver = new DatagramSocket();
	bool result = receiver->bindToPort(localPort->intValue());

	if (result)
	{
		NLOG(niceName, "Now receiving on port : " + localPort->stringValue());
	} else
	{
		NLOG(niceName, "Error binding port " + localPort->stringValue());
	}

	startThread();

	Array<IPAddress> ad;
	IPAddress::findAllAddresses(ad);

	String s = "Local IPs:";
	for (auto &a : ad) s += String("\n > ") + a.toString();
	NLOG(niceName, s);
}

void UDPModule::processMessage(const String & msg)
{
	if (logIncomingData->boolValue())
	{
		NLOG(niceName, msg);
	}

	inActivityTrigger->trigger();
	processMessageInternal(msg);

	scriptManager->callFunctionOnAllItems(udpEventId, msg);
}


void UDPModule::setupSender()
{
	if (sendCC == nullptr) return;

	sender = nullptr;
	if (!sendCC->enabled->boolValue()) return;

	sender = new DatagramSocket();
}



void UDPModule::sendMessage(const String & msg)
{
	if (!enabled->boolValue()) return;
	if (sendCC == nullptr) return;

	if (!sendCC->enabled->boolValue()) return;

	if (logOutgoingData->boolValue()) NLOG(niceName, "Send UDP : " << msg);

	outActivityTrigger->trigger();
	
	String targetHost = useLocal->boolValue() ? "127.0.0.1" : remoteHost->stringValue(); 
	sender->write(targetHost,remotePort->intValue(),msg.getCharPointer(),msg.length());
}

var UDPModule::sendMessageFromScript(const var::NativeFunctionArgs & a)
{
	UDPModule * m = getObjectFromJS<UDPModule>(a);
	if (!m->enabled->boolValue()) return var();

	if (a.numArguments == 0) return var();

	String msg(a.arguments[0].toString());

	m->sendMessage(msg);

	return var();
}

var UDPModule::getJSONData()
{
	var data = Module::getJSONData();
	if (receiveCC != nullptr) data.getDynamicObject()->setProperty("input", receiveCC->getJSONData());
	if (sendCC != nullptr) data.getDynamicObject()->setProperty("output", sendCC->getJSONData());
	return data;
}

void UDPModule::loadJSONDataInternal(var data)
{
	Module::loadJSONDataInternal(data);
	if (receiveCC != nullptr) receiveCC->loadJSONData(data.getProperty("input", var()));
	if (sendCC != nullptr) sendCC->loadJSONData(data.getProperty("output", var()));
}



void UDPModule::controllableFeedbackUpdate(ControllableContainer * cc, Controllable * c)
{
	Module::controllableFeedbackUpdate(cc, c);

	if (receiveCC != nullptr && c == receiveCC->enabled)
	{
		bool rv = receiveCC->enabled->boolValue();
		bool sv = sendCC->enabled->boolValue();
		setupIOConfiguration(rv, sv);
		localPort->setEnabled(rv);
		setupReceiver();

	} else if (sendCC != nullptr && c == sendCC->enabled)
	{
		bool rv = receiveCC->enabled->boolValue();
		bool sv = sendCC->enabled->boolValue();
		setupIOConfiguration(rv, sv);
		remoteHost->setEnabled(sv);
		remotePort->setEnabled(sv);
		useLocal->setEnabled(sv);
		setupSender();

	} else if (c == localPort) setupReceiver();
	else if (c == remoteHost || c == remotePort || c == useLocal)
	{
		setupSender();
		if (c == useLocal) remoteHost->setEnabled(!useLocal->boolValue());
	}
}


void UDPModule::run()
{
	receiver->waitUntilReady(false, 100);
	char buffer[512];

	String stringBuffer = ""; //for lines;
	Array<uint8> byteBuffer; //for cobs and data255

	while (!threadShouldExit())
	{
		sleep(10); //100fps

		if (receiver == nullptr) return;
		
		try
		{

			size_t numBytes = receiver->read(buffer, 512, false);
			if (numBytes == 0) continue;

			Mode m = modeParam->getValueDataAsEnum<Mode>();
			switch (m)
			{

			case LINES:
			{
				stringBuffer.append(String::fromUTF8(buffer, (int)numBytes),numBytes);
				StringArray sa;
				sa.addTokens(stringBuffer, "\n", "\"");
				for (int i = 0; i < sa.size() - 1; i++) processMessage(sa[i]);
				stringBuffer = sa[sa.size() - 1];
			}
			break;

			case RAW:
			{
				processMessage(String::fromUTF8(buffer, (int)numBytes));
			}
			break;

			case DATA255:
			{
				for (int i = 0; i < numBytes; i++)
				{
					uint8 b = buffer[i];
					if (b == 255)
					{
						//processBytes here
						byteBuffer.clear();
					} else
					{
						byteBuffer.add(b);
					}
				}
				
			}
			break;

			case COBS:
			{
				/*
				for (int i = 0; i < numBytes; i++)
				{
					uint8_t b = port->port->read(1)[0];
					byteBuffer.push_back(b);
					if (b == 0)
					{
						uint8_t decodedData[255];
						size_t numDecoded = cobs_decode(byteBuffer.data(), byteBuffer.size(), decodedData);
						serialThreadListeners.call(&SerialThreadListener::newMessage, var(decodedData, numDecoded));
						byteBuffer.clear();
					}
				}
				*/
			}
			break;
			}
		} catch (...)
		{
			DBG("### UDP Problem ");
		}
	}
}
