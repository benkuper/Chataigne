/*
  ==============================================================================

	PJLinkModule.cpp
	Created: 5 Jan 2018 3:41:58pm
	Author:  Ben

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

PJLinkModule::PJLinkModule() :
	Module(getDefaultTypeString()),
	Thread("PJLink"),
	clientsParamsCC("Projectors"),
	clientsValuesCC("Projectors"),
	autoRequestIsPower(true)
{
	alwaysShowValues = true;
	includeValuesInSave = true;

	numClients = moduleParams.addIntParameter("Num Projectors", "Number of projectors to control", 1, 1, 100);
	autoRequestTime = moduleParams.addIntParameter("Auto Request Timer", "If enabled, this is the number of seconds between auto request, alternating power and shutter status", 5, 1, 100, false);
	autoRequestTime->canBeDisabledByUser = true;

	allProjectorsPoweredOn = valuesCC.addBoolParameter("All Powered On", "Are all projectors powered on ?", false);
	allProjectorsPoweredOff = valuesCC.addBoolParameter("All Powered Off", "Are all projectors powered off ?", false);


	moduleParams.addChildControllableContainer(&clientsParamsCC);
	valuesCC.addChildControllableContainer(&clientsValuesCC);
	clientsParamsCC.saveAndLoadRecursiveData = true;
	clientsValuesCC.saveAndLoadRecursiveData = true;

	allClientsAreConnected = moduleParams.addBoolParameter("All Connected", "Are all enabled client connected ?", false);
	allClientsAreConnected->setControllableFeedbackOnly(true);
	allClientsAreConnected->hideInEditor = true;
	connectionFeedbackRef = allClientsAreConnected;


	defManager->add(CommandDefinition::createDef(this, "", "Power", &PJLinkCommand::create, CommandContext::BOTH)->addParam("action", PJLinkCommand::POWER));
	defManager->add(CommandDefinition::createDef(this, "", "Shutter", &PJLinkCommand::create, CommandContext::BOTH)->addParam("action", PJLinkCommand::SHUTTER));
	defManager->add(CommandDefinition::createDef(this, "", "Custom", &PJLinkCommand::create, CommandContext::BOTH)->addParam("action", PJLinkCommand::CUSTOM));

	updateClientsSetup();

	startTimer(autoRequestTime->intValue() * 1000);
	startThread();
}

PJLinkModule::~PJLinkModule()
{
	stopThread(1000);
}

void PJLinkModule::updateClientsSetup()
{
	int numC = numClients->intValue();
	while (numC < clients.size())
	{
		removeClient(clients[clients.size() - 1]);
	}

	while (numC > clients.size())
	{
		addClient();
	}

	updateAllPoweredStatuses();
}

PJLinkModule::PJLinkClient* PJLinkModule::addClient()
{
	PJLinkClient* c = new PJLinkClient(this, clients.size() + 1);
	clientsParamsCC.addChildControllableContainer(&c->paramsCC);
	clientsValuesCC.addChildControllableContainer(&c->valuesCC);
	clients.add(c);
	return c;
}

void PJLinkModule::removeClient(PJLinkClient* c)
{
	if (c != nullptr)
	{
		clientsParamsCC.removeChildControllableContainer(&c->paramsCC);
		clientsValuesCC.removeChildControllableContainer(&c->valuesCC);
	}

	clients.removeObject(c);
}

void PJLinkModule::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	Module::onControllableFeedbackUpdateInternal(cc, c);

	if (c == numClients) updateClientsSetup();
	else if (c == autoRequestTime) startTimer(autoRequestTime->intValue() * 1000);
	else if (cc == &valuesCC)
	{
		ControllableContainer* pc = c->parentContainer.get();
		for (auto& client : clients)
		{
			if (pc == &client->valuesCC)
			{
				if (c == client->shutterAudioStatus || c == client->shutterVideoStatus)
				{
					if (!client->assigningFromRemote)
					{
						if (c == client->shutterVideoStatus) sendMessageToClient("%1AVMT 1" + String(client->shutterVideoStatus->intValue()), client->id);
						else if (c == client->shutterAudioStatus) sendMessageToClient("%1AVMT 2" + String(client->shutterAudioStatus->intValue()), client->id);
					}
				}
				else if (c == client->powerStatus)
				{
					updateAllPoweredStatuses();
				}
			}
		}
	}
	else if (cc == &moduleParams)
	{
		ControllableContainer* pc = c->parentContainer.get();
		for (auto& client : clients)
		{
			if (pc == &client->paramsCC)
			{
				if (c == client->remoteHost || c == client->remotePort || c == client->paramsCC.enabled)
				{
					client->setupClient();
				}

				if (c == client->isConnected || c == client->paramsCC.enabled)
				{
					updateConnectedStatus();
				}
			}
		}
	}
}

void PJLinkModule::updateConnectedStatus()
{
	for (auto& c : clients)
	{
		if (!c->paramsCC.enabled->boolValue()) continue;
		if (!c->isConnected->boolValue())
		{
			allClientsAreConnected->setValue(false);
			return;
		}
	}

	allClientsAreConnected->setValue(true);

	updateAllPoweredStatuses();
}


void PJLinkModule::updateAllPoweredStatuses()
{
	bool off = true;
	bool on = true;

	for (auto& c : clients)
	{
		if (!c->paramsCC.enabled->boolValue()) continue;
		if (!c->isConnected->boolValue()) on = false;

		String s = c->powerStatus->getValueData().toString();
		bool poweredOff = s == "0";
		bool poweredOn = s == "1";

		if (!poweredOn) on = false;
		if (!poweredOff) off = false;
	}

	allProjectorsPoweredOn->setValue(on);
	allProjectorsPoweredOff->setValue(off);
}


void PJLinkModule::sendMessageToClient(const String& message, int id)
{
	if (!enabled->boolValue()) return;

	outActivityTrigger->trigger();

	if (id == -1)
	{
		for (int i = 1; i <= clients.size(); i++) sendMessageToClient(message, i);
		return;
	}

	if (id <= 0 || id > clients.size())  return;

	PJLinkClient* client = clients[id - 1];
	if (client == nullptr) return;

	String msg = message + "\r";

	String encodedPass = juce::MD5((client->passBytes + client->password->stringValue()).toUTF8()).toHexString();
	String encodedMessage = client->password->stringValue().isEmpty() ? msg : (encodedPass + msg);

	if (client->isConnected->boolValue() && client->paramsCC.enabled->boolValue())
	{
		int numWritten = client->client.write(encodedMessage.toStdString().c_str(), encodedMessage.length());
		if (numWritten == -1)
		{
			NLOGERROR(niceName, "Error writing to client " << client->id);
			client->isConnected->setValue(false);
		}
		else
		{
			if (logOutgoingData->boolValue()) NLOG(niceName, message << " sent to projector " << client->id);
		}
	}
	else
	{
		if (logOutgoingData->boolValue()) NLOGWARNING(niceName, "Did not send message to : " << message << ", client " << client->id << " is not connected");
	}
}


void PJLinkModule::timerCallback()
{
	if (!autoRequestTime->enabled) return;

	for (auto& c : clients)
	{
		if (c == nullptr) return;
		if (!c->isConnected->boolValue() || !c->lastRequestReplied) c->setupClient();
		c->lastRequestReplied = false;
	}

	sendMessageToClient(autoRequestIsPower ? "%1POWR ?" : "%1AVMT ?");

	autoRequestIsPower = !autoRequestIsPower;

}

void PJLinkModule::run()
{
	while (!threadShouldExit())
	{
		wait(100);
		for (auto& c : clients) processClient(c);
	}
}

void PJLinkModule::processClient(PJLinkModule::PJLinkClient* c)
{
	if (c == nullptr) return;
	if (!c->isConnected->boolValue()) return;
	if (!c->paramsCC.enabled->boolValue()) return;

	uint8 bytes[2048];
	int numRead = c->client.read(bytes, 2048, false);

	//if (numRead == 0)
	//{
	//	NLOGWARNING(niceName, "Connection to TCP Server seems lost, disconnecting");
	//	c->isConnected->setValue(false);
	//}

	if (CharPointer_UTF8::isValidString((char*)bytes, numRead))
	{
		c->stringBuffer.append(String::fromUTF8((char*)bytes, numRead), numRead);
		StringArray sa;
		sa.addTokens(c->stringBuffer, "\r\n", "\"");
		for (int i = 0; i < sa.size() - 1; ++i) processClientLine(c, sa[i]);
		c->stringBuffer = sa[sa.size() - 1];
	}
}
void PJLinkModule::processClientLine(PJLinkClient* c, const String& message)
{

	inActivityTrigger->trigger();
	if (logIncomingData->boolValue()) NLOG(niceName, "Received : " << message);

	if (message.contains("PJLINK"))
	{
		c->lastRequestReplied = true;

		StringArray mSplit;
		mSplit.addTokens(message, true);
		if (mSplit[1] == "1")
		{
			if (mSplit.size() >= 3)
			{
				c->passBytes = mSplit[2];
				if (logIncomingData->boolValue()) NLOG(niceName, "> PJLINK handshake with password gen key " << mSplit[2]);
			}
		}
		else if (mSplit[1] == "0")
		{
			if (logIncomingData->boolValue()) NLOG(niceName, "> PJLINK handshake, projector is not password protected");
		}
		else if (mSplit[1].contains("ERR"))
		{
			NLOGERROR(niceName, "PJLINK Authentication error for projector " << c->id << ", please verify password !");
		}
	}
	else if (message.contains("%1POWR="))
	{
		String status = message.substring(7);
		if (status != "OK")
		{
			if (logIncomingData->boolValue()) NLOG(niceName, " > Received Power status " << status);
			c->powerStatus->setValueWithData(status);
		}
		else
		{
			if (logIncomingData->boolValue()) NLOG(niceName, " > Project power command accepted !");
		}
	}
	else if (message.contains("%1AVMT="))
	{
		bool isVideo = message[7] == '1' || message[7] == '3';
		bool isAudio = message[7] == '2' || message[7] == '3';
		bool isOn = message[8] == '1';

		c->assigningFromRemote = true;
		if (isVideo)
		{
			if (logIncomingData->boolValue()) NLOG(niceName, " > Received Video shutter status " << (isOn ? "ON" : "OFF"));
			c->shutterVideoStatus->setValue(isOn);
		}

		if (isAudio)
		{
			if (logIncomingData->boolValue()) NLOG(niceName, " > Received Audio shutter status " << (isOn ? "ON" : "OFF"));
			c->shutterAudioStatus->setValue(isOn);
		}
		c->assigningFromRemote = false;
	}
}

PJLinkModule::PJLinkClient::PJLinkClient(PJLinkModule* m, int id) :
	Thread("PJLink Client " + String(id)),
	pjlinkModule(m),
	id(id),
	passBytes(0),
	assigningFromRemote(false),
	paramsCC("Projector " + String(id)),
	valuesCC("Projector " + String(id))
{

	std::function<InspectableEditor* (bool, Array<ControllableContainer*>)> func = [&](bool, Array<ControllableContainer*>) { return new PJLinkClientParamContainerEditor(this); };
	paramsCC.customGetEditorFunc = func;

	remoteHost = paramsCC.addStringParameter("IP", "IP of the projector", "192.168.1.100");
	remotePort = paramsCC.addIntParameter("Port", "Port of the projector", 4352, 1, 65535);
	isConnected = paramsCC.addBoolParameter("Is Connected", "Is this projector connected", false);
	isConnected->setControllableFeedbackOnly(true);
	isConnected->hideInEditor = true;

	password = paramsCC.addStringParameter("Password", "Password for the PJLink, leave empty if no pass set up on the Projector", "");

	powerStatus = valuesCC.addEnumParameter("Power Status", "Status of the projector");
	powerStatus->addOption("Not set", -1)->addOption("Powering OFF", "0")->addOption("Powering ON", "1")->addOption("Cooling", "2")->addOption("Warming Up", "3")->addOption("Unavailable Time", "ERR3")->addOption("Projector / Display Failure", "ERR4");

	shutterVideoStatus = valuesCC.addBoolParameter("Shutter Video Status", "Is the projector's shutter on", false);
	shutterAudioStatus = valuesCC.addBoolParameter("Shutter Audio Status", "Is the projector's shutter on", false);

}

PJLinkModule::PJLinkClient::~PJLinkClient()
{
	stopThread(1000);
}

void PJLinkModule::PJLinkClient::setupClient()
{
	startThread();
}

void PJLinkModule::PJLinkClient::run()
{
	if (client.isConnected())
	{
		client.close();
	}

	isConnected->setValue(false);

	if (!paramsCC.enabled->boolValue()) return;

	if (pjlinkModule->logOutgoingData->boolValue()) LOG("Connecting to " << remoteHost->stringValue() << ":" << remotePort->intValue() << "...");

	bool result = client.connect(remoteHost->stringValue(), remotePort->intValue(), 500);
	if (!result)
	{

		if (paramsCC.getWarningMessage().isEmpty()) paramsCC.setWarningMessage("Could not connect to " + remoteHost->stringValue() + ":" + remotePort->stringValue());
		return;
	}

	isConnected->setValue(true);
	paramsCC.clearWarning();

	if (pjlinkModule->logOutgoingData->boolValue()) LOG("Connected to projector " << (id) << " (" << remoteHost->stringValue() << ":" << remotePort->intValue() << ")");
}
