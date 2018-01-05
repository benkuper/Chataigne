/*
  ==============================================================================

    TCPModule.cpp
    Created: 21 Oct 2017 5:04:54pm
    Author:  Ben

  ==============================================================================
*/

#include "TCPModule.h"
#include "SendTCPStringCommand.h"
#include "SendTCPRawDataCommand.h"

TCPModule::TCPModule(const String & name, int defaultRemotePort) :
	Module(name),
	Thread(name)
{
	sender = new StreamingSocket();

	setupIOConfiguration(true, true);

	canHandleRouteValues = true;

	//Send
	useLocal = moduleParams.addBoolParameter("Local", "Connect to Local IP (127.0.0.1). Allow to quickly switch between local and remote IP.", true);
	remoteHost = moduleParams.addStringParameter("Remote Host", "Remote Host to send to.", "127.0.0.1");
	remotePort = moduleParams.addIntParameter("Remote port", "Port on which the remote host is listening to", defaultRemotePort, 1024, 65535);

	isConnected = moduleParams.addBoolParameter("Is Connected", "Is the receiver connected the the remote port", false);
	isConnected->isEditable = false;
	isConnected->hideInOutliner = true;
	isConnected->isTargettable = false;
	isConnected->isSavable = false;

	modeParam = moduleParams.addEnumParameter("Mode", "Protocol for treating the incoming data");
	modeParam->addOption("Lines", LINES)->addOption("Raw", RAW)->addOption("Data255", DATA255)->addOption("COBS", COBS);

	setupSender();
	
	reconnectRemote = moduleParams.addTrigger("Reconnect remote", "If remote is disconnected, tries to connect again");

	//Commands
	defManager.add(CommandDefinition::createDef(this, "", "Send string", &SendTCPStringCommand::create, CommandContext::ACTION));
	defManager.add(CommandDefinition::createDef(this, "", "Send raw data", &SendTCPRawDataCommand::create, CommandContext::ACTION));

	//Script
	scriptObject.setMethod(sendTCPId, TCPModule::sendMessageFromScript);
	scriptObject.setMethod(writeTCPId, TCPModule::writeDataFromScript);

}

TCPModule::~TCPModule()
{
	DBG("Delete");
	signalThreadShouldExit();
	while (isThreadRunning());
	DBG("Close sender");
	sender->close();
	DBG("Delete sender");
	sender = nullptr;
}


void TCPModule::setupSender()
{
	sender->close();
	signalThreadShouldExit();
	while (isThreadRunning());
	startThread();
}

void TCPModule::sendStringPacket(const String &s)
{
	DBG("Here sending packet");
	if (!enabled->boolValue()) return;
	if (!sender->isConnected()) return;


	 outActivityTrigger->trigger();
	 if (logOutgoingData->boolValue())
	 {
		 String rh = useLocal->boolValue() ? "127.0.0.1" : remoteHost->stringValue();
		 NLOG(niceName, "Sending " << s << " to " << rh << ":" << remotePort->intValue());
	 }

	sender->write(s.getCharPointer(), s.length());
	//sender.close();
}


void TCPModule::sendRawData(Array<uint8> data)
{
	DBG("Here send raw data");
	if (!enabled->boolValue()) return;
	if (!sender->isConnected()) return;

	outActivityTrigger->trigger();
	if (logOutgoingData->boolValue())
	{
		String rh = useLocal->boolValue() ? "127.0.0.1" : remoteHost->stringValue();
		NLOG(niceName, "Sending " << data.size() << " bytes to " << rh << ":" << remotePort->intValue());
	}

	sender->write(data.getRawDataPointer(), data.size());
}


void TCPModule::processRawData(Array<uint8> data)
{
	if (logIncomingData->boolValue())
	{
		NLOG(niceName, "Received " + String(data.size()) + " bytes :");
		for (auto &b : data) LOG(String(b));
	}

	inActivityTrigger->trigger();
	
	var bData = var();
	for (auto &b : data) bData.append(b);
	scriptManager->callFunctionOnAllItems(tcpEventId, bData);
}



void TCPModule::processMessage(const String & msg)
{
	if (logIncomingData->boolValue())
	{
		NLOG(niceName, "Received message :\n"+msg);
	}

	inActivityTrigger->trigger();
	processMessageInternal(msg);

	scriptManager->callFunctionOnAllItems(tcpEventId, msg);
}

void TCPModule::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c)
{
	Module::onControllableFeedbackUpdateInternal(cc, c);
	if (c== remoteHost || c == remotePort || c == useLocal) setupSender();
	if (c == useLocal) remoteHost->setEnabled(!useLocal->boolValue());
	if (c == reconnectRemote) setupSender();
}



var TCPModule::sendMessageFromScript(const var::NativeFunctionArgs & a)
{
	TCPModule * m = getObjectFromJS<TCPModule>(a);
	if (!m->enabled->boolValue()) return var();

	if (a.numArguments == 0) return var();

	String msg(a.arguments[0].toString());

	m->sendStringPacket(msg);

	return var();
}

var TCPModule::writeDataFromScript(const var::NativeFunctionArgs & a)
{
	TCPModule * m = getObjectFromJS<TCPModule>(a);
	if (!m->enabled->boolValue()) return var();

	if (a.numArguments == 0) return var();

	Array<uint8> data;
	for (int i = 0; i < a.numArguments; i++) data.add((uint8)(int)a.arguments[i]);

	m->sendRawData(data);

	return var();
}

void TCPModule::run()
{
	String targetHost = useLocal->boolValue() ? "127.0.0.1" : remoteHost->stringValue();
	NLOG(niceName, "Connecting to " + targetHost + ":"+ String(remotePort->intValue())+"...");
	bool result = sender->connect(targetHost, remotePort->intValue(),500);
	
	isConnected->setValue(result);
	if (result)
	{
		NLOG(niceName, "Connected !");
	} else
	{
		NLOGWARNING(niceName, "Could not connect");
		return;
	}

	char buffer[512];
	String stringBuffer = ""; //for lines;
	Array<uint8> byteBuffer; //for cobs and data255

	
	while (!threadShouldExit())
	{
		sleep(10); //100 fps

		if (sender->isConnected())
		{
			int ready = sender->waitUntilReady(true, 300);
			if (ready == 1)
			{
				try
				{
					int numBytes = sender->read(buffer, 512, false);
					if (numBytes == 0) continue;

					Mode m = modeParam->getValueDataAsEnum<Mode>();
					switch (m)
					{

					case LINES:
					{
						stringBuffer.append(String::fromUTF8(buffer, (int)numBytes), numBytes);
						StringArray sa;
						sa.addTokens(stringBuffer, "\n", "\"");
						for (int i = 0; i < sa.size() - 1; i++) processMessage(sa[i]);
						stringBuffer = sa[sa.size() - 1];
					}
					break;

					case RAW:
					{
						processRawData(Array<uint8>(buffer, (int)numBytes));
					}
					break;

					case DATA255:
					{
						for (int i = 0; i < numBytes; i++)
						{
							uint8 b = buffer[i];
							if (b == 255)
							{
								processRawData(byteBuffer);
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
					DBG("### TCP Problem ");
				}

			}
			
		}
	}
}
