/*
  ==============================================================================

	PJLinkModule.cpp
	Created: 5 Jan 2018 3:41:58pm
	Author:  Ben

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"
#include "PJLinkModule.h"

PJLinkModule::PJLinkModule() :
	StreamingModule(getDefaultTypeString()),
	Thread("PJLink"),
	clientsParamsCC("Projectors"),
	clientsValuesCC("Projectors"),
	autoRequestIsPower(true)
{
	alwaysShowValues = true;
	includeValuesInSave = true;
	valuesCC.userCanAddControllables = true;
	valuesCC.customUserCreateControllableFunc = nullptr;
	setAutoAddAvailable(false);

	scriptManager->scriptTemplate += ChataigneAssetManager::getInstance()->getScriptTemplate("pjlink");

	numClients = moduleParams.addIntParameter("Num Projectors", "Number of projectors to control", 1, 1, 100);
	//numClients->alwaysNotify = true;
	//numClients->forceSaveValue = true;

	autoRequestTime = moduleParams.addIntParameter("Auto Request Timer", "If enabled, this is the number of seconds between auto request, alternating power and shutter status", 5, 1, 100, true);
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

	//var customParams;
	//DynamicObject* o(new DynamicObject());
	//o->setProperty("index", 0);
	//o->setProperty("name", "Projector ID");
	//o->setProperty("type", IntParameter::getTypeStringStatic());
	//o->setProperty("default", 1);
	//o->setProperty("min", 1);
	//o->setProperty("canBeDisabled", true);
	//o->setProperty("enabled", false);
	//customParams.append(o);

	//for (auto& d : defManager->definitions) d->addParam("customParams", customParams);

	defManager->definitions.clear();

	//defManager->add(CommandDefinition::createDef(this, "", "Send string", &SendStreamStringCommand::create, CommandContext::BOTH)->addParam("customParams", customParams)->addParam("hideNLCR",true));
	//defManager->add(CommandDefinition::createDef(this, "", "Send values as string", &SendStreamStringValuesCommand::create, CommandContext::BOTH)->addParam("customParams", customParams)->addParam("hideNLCR", true));


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

	if (isCurrentlyLoadingData)
	{

		for (int i = 0; i < clients.size(); i++)
		{
			if (ghostClientNames.size() > i) clients[i]->paramsCC.setNiceName(ghostClientNames[i]);
		}
	}

	updateAllPoweredStatuses();
}

PJLinkModule::PJLinkClient* PJLinkModule::addClient()
{
	PJLinkClient* c = new PJLinkClient(this, clients.size() + 1);
	clientsParamsCC.addChildControllableContainer(&c->paramsCC);
	clientsValuesCC.addChildControllableContainer(&c->valuesCC);

	if (isCurrentlyLoadingData && ghostClientNames.size() > clients.size())
	{
		c->paramsCC.setNiceName(ghostClientNames[clients.size()]);
	}

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

			if (c == client->inputStatus)
			{
				StringArray chooseInput(client->inputStatus->getValue());
				if (!(chooseInput[0] == "-1") && !(chooseInput[0] == ""))
				{
					String messageToSendInput = "%1INPT ";
					messageToSendInput.append(chooseInput[0], 20);

					sendMessageToClient(messageToSendInput, client->id);
					sendMessageToClient("%2IRES ?", client->id);
				}
			}

			if (c == client->updateInput)
			{
				sendMessageToClient("%1INPT ?", client->id);
				wait(500);
				sendMessageToClient("%2IRES ?", client->id);
			}

			if (c == client->updateInfo) requestInfos();
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

void PJLinkModule::sendMessageInternal(const String& message, var params)
{
	sendMessageToClient(message, params.getProperty("projectorID", -1));
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

void PJLinkModule::requestInfos()
{
	for (int i = 1; i <= clients.size(); i++)
	{
		sendMessageToClient("%1INST ?", i);		// input list
		wait(100);
		sendMessageToClient("%2IRES ?", i);		// Input resolution
		wait(100);
		sendMessageToClient("%2RRES ?", i);		// Recommended resolution
		wait(100);
		sendMessageToClient("%1INF1 ?", i);		// Manufacturer name
		wait(100);
		sendMessageToClient("%1INF2 ?", i);		// Product name
		wait(100);
		sendMessageToClient("%1NAME ?", i);		// Display name
		wait(100);
		sendMessageToClient("%2SVER ?", i);		// Firmware version
		wait(100);
		sendMessageToClient("%1LAMP ?", i);		// Lamp hours
		wait(100);
		sendMessageToClient("%2RLMP ?", i);		// Lamp model
		wait(100);
		sendMessageToClient("%2FILT ?", i);		// Filter time
		wait(100);
		sendMessageToClient("%2RFIL ?", i);		// Filter number
		wait(100);
		sendMessageToClient("%1ERST ?", i);		// chek errors
	}
}
void PJLinkModule::processClient(PJLinkModule::PJLinkClient* c)
{
	if (!enabled->boolValue()) return;
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

	c->lastRequestReplied = true;

	Array<var> args;
	args.add(c->id);
	args.add(message);
	scriptManager->callFunctionOnAllItems(pjLinkDataReceivedId, args);

	if (message.contains("PJLINK"))
	{
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
			if (logIncomingData->boolValue())
			{
				NLOG(niceName, " > Project power command accepted !");

				sendMessageToClient("%1INPT ?", c->id);
				sendMessageToClient("%2IRES ?", c->id);
			}
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
	else if (message.contains("%1INF1="))    // Manufacturer name
	{
		if (message.substring(7) == "ERR3") NLOGERROR(niceName, " Error while receiving input name : Unavailable time for any reason ");
		else if (message.substring(7) == "ERR4") NLOGERROR(niceName, " Error while receiving input name : Projector/Display failure ");
		else
		{
			c->brendInfo->setValue(message.substring(7));

			NLOG(niceName, " > Received Manufacturer name of projector" << c->id);
		}
	}
	else if (message.contains("%1INF2="))    // Product name
	{
		if (message.substring(7) == "ERR3") NLOGERROR(niceName, " Error while receiving product name : Unavailable time for any reason ");
		else if (message.substring(7) == "ERR4") NLOGERROR(niceName, " Error while receiving product name : Projector/Display failure ");
		else
		{
			c->productNameInfo->setValue(message.substring(7));

			NLOG(niceName, " > Received product name of projector " << c->id);
		}
	}
	else if (message.contains("%1NAME="))    // Display name
	{
		if (message.substring(7) == "ERR3")	NLOGERROR(niceName, " Error while receiving display name : Unavailable time for any reason ");
		else if (message.substring(7) == "ERR4") NLOGERROR(niceName, " Error while receiving display name : Projector/Display failure ");
		else
		{
			c->displayNameInfo->setValue(message.substring(7));
			NLOG(niceName, " > Received display name of projector " << c->id);
		}
	}
	else if (message.contains("%2IRES="))    // Input resolution
	{
		if (message.substring(7) == "ERR3")	NLOGERROR(niceName, " Error while receiving input resolution : Unavailable time (maybe the projector is OFF)");
		else if (message.substring(7) == "ERR4") NLOGERROR(niceName, " Error while receiving input resolution : Projector/Display failure ");
		else
		{
			c->inputResolutionInfo->setValue(message.substring(7));

			NLOG(niceName, " > Received input resolution of projector " << c->id);
		}
	}
	else if (message.contains("%2RRES="))    // Recommended resolution
	{
		if (message.substring(7) == "ERR3")	NLOGERROR(niceName, " Error while receiving recommended resolution : Unavailable time for any reason ");
		else if (message.substring(7) == "ERR4") NLOGERROR(niceName, " Error while receiving recommended resolution : Projector/Display failure ");
		else
		{
			c->resolutionRecommendedInfo->setValue(message.substring(7));

			NLOG(niceName, " > Received recommmended reolution of projector " << c->id);
		}
	}
	else if (message.contains("%2RLMP="))    // Lamp model
	{
		if (message.substring(7) == "ERR3")	NLOGERROR(niceName, " Error while receiving lamp model : Unavailable time for any reason ");
		else if (message.substring(7) == "ERR4") NLOGERROR(niceName, " Error while receiving lamp model : Projector/Display failure ");
		else if (message.substring(7) == "")
		{
			c->lampModelInfo->setValue("No information");

			NLOG(niceName, " > Received empty lamp model of projector " << c->id);
		}
		else
		{
			c->lampModelInfo->setValue(message.substring(7));

			NLOG(niceName, " > Received lamp model of projector " << c->id);
		}
	}
	else if (message.contains("%2FILT="))    // Filter time
	{
		if (message.substring(7) == "ERR1")	c->filterUsageInfo->setValue(message.substring(7) + "No filter");
		else if (message.substring(7) == "ERR3") NLOGERROR(niceName, " Error while receiving filter time : Unavailable time for any reason ");
		else if (message.substring(7) == "ERR4") NLOGERROR(niceName, " Error while receiving filter time : Projector/Display failure ");
		else
		{
			c->filterUsageInfo->setValue(message.substring(7) + " hours");

			NLOG(niceName, " > Received filter time of projector " << c->id);
		}
	}
	else if (message.contains("%2RFIL="))    // Filter number
	{
		if (message.substring(7) == "")	c->filterModelInfo->setValue(message.substring(7) + "No filter");
		else if (message.substring(7) == "ERR3") NLOGERROR(niceName, " Error while receiving filter number : Unavailable time for any reason ");
		else if (message.substring(7) == "ERR4") NLOGERROR(niceName, " Error while receiving filter number : Projector/Display failure ");
		else
		{
			c->filterModelInfo->setValue(message.substring(7));

			NLOG(niceName, " > Received filter number of projector " << c->id);
		}
	}
	else if (message.contains("%2SVER="))    // firmware version
	{
		if (message.substring(7) == "") c->softVerInfo->setValue(message.substring(7) + "No firmware information ");
		else if (message.substring(7) == "ERR3") NLOGERROR(niceName, " Error while receiving firmware information : Unavailable time for any reason ");
		else if (message.substring(7) == "ERR4") NLOGERROR(niceName, " Error while receiving firmware information : Projector/Display failure ");
		else
		{
			c->softVerInfo->setValue(message.substring(7));

			NLOG(niceName, " > Received firmware version of projector " << c->id);
		}
	}
	else if (message.contains("%1LAMP="))    // Lamp hours
	{
		if (message.substring(7) == "ERR1") NLOGERROR(niceName, " Error while receiving lamp hours : No lamp ");
		else if (message.substring(7) == "ERR3") NLOGERROR(niceName, " Error while receiving lamp hours : Unvailable time ");
		else if (message.substring(7) == "ERR4") NLOGERROR(niceName, " Error while receiving lamp hours : Projector/Display failure ");
		else
		{
			String lhs("");
			StringArray ls;

			ls.addTokens(message.substring(message.indexOfIgnoreCase("=") + 1), " ");

			const int& nbVp(ls.size());

			for (int i = 0; i < ls.size() - 1; i += 2)
			{
				lhs += " lamp " + String(i + 1);
				lhs += " = " + ls[i];

				if (!(i == nbVp - 2)) lhs += ", ";
			}

			c->lampHourInfo->setValue(lhs);

			NLOG(niceName, " > Received lamp hours of projector " << c->id);
		}
	}

	else if (message.contains("%1INST="))    // input list
	{
		c->inputStatus->clearOptions();
		c->inputStatus->addOption("Not set", -1);

		if (message.substring(7) == "ERR3") NLOGERROR(niceName, " Error while receiving the input list : Unavailable time for any reason ");
		else if (message.substring(7) == "ERR4") NLOGERROR(niceName, " Error while receiving the input list : Projector/Display failure ");
		else if (!(message.substring(7) == ""))
		{
			inputListVp.clear();
			inputListVp.addTokens(message.substring(message.indexOfIgnoreCase("=") + 1), " ");

			int i(0);

			for (i = 0; i < (inputListVp.size()); i += 1)
			{
				sendMessageToClient("%2INNM ?" + inputListVp[i], c->id);
			}

			NLOG(niceName, " > Received input list of projector " << c->id);

			requestInputName(c->id);
		}
	}
	else if (message.contains("%2INNM="))    //  input name
	{
		if (message.substring(7) == "ERR2") NLOGERROR(niceName, " Error while receiving input name : Out of parameter");
		else if (message.substring(7) == "ERR3")
		{
			NLOGERROR(niceName, " Error while receiving input name : Unvailable time");

		}
		else if (message.substring(7) == "ERR4") NLOGERROR(niceName, " Error while receiving input name : Projector/display failure");
		else

		{
			c->inputStatus->addOption(message.substring(7), inputListVp[indexInput]);
		}



		NLOG(niceName, " > Received input name of projector " << c->id);

		indexInput += 1;

		requestInputName(c->id);
	}

	else if (message.contains("%1INPT="))		// Input
	{
		if (message.substring(7) == "ERR2")
		{
			NLOGERROR(niceName, " Error nonexistent input source ");
			c->inputStatus->setValueWithKey("-1");
		}
		else if (message.substring(7) == "ERR3")
		{
			NLOGERROR(niceName, " Error while receiving recommended resolution : Unavailable time for any reason ");
			c->inputStatus->setValueWithKey("-1");
		}
		else if (message.substring(7) == "ERR4")
		{
			NLOGERROR(niceName, " Error while receiving recommended resolution : Projector/Display failure ");
			c->inputStatus->setValueWithKey("-1");
		}
		else
		{
			c->inputStatus->setValueWithData(message.substring(7));
			NLOG(niceName, " > Received input of projector " << c->id);
		}
	}

	else if (message.contains("%1ERST="))	//	errors	
	{
		if (message.substring(7) == "ERR3") { NLOGERROR(niceName, " Error while receiving errors : Unvailable time"); }
		else if (message.substring(7) == "ERR4") { NLOGERROR(niceName, " Error while receiving errors : Projector/display failure"); }
		else if (!(message.substring(7) == ""))
		{
			c->fanErrorInfo->setValue(PJLinkModule::convertError(message.substring(7, 8)));
			c->lampErrorInfo->setValue(PJLinkModule::convertError(message.substring(8, 9)));
			c->temperatureErrorInfo->setValue(PJLinkModule::convertError(message.substring(9, 10)));
			c->coverErrorInfo->setValue(PJLinkModule::convertError(message.substring(10, 11)));
			c->filterErrorInfo->setValue(PJLinkModule::convertError(message.substring(11, 12)));
			c->otherErrorInfo->setValue(PJLinkModule::convertError(message.substring(12, 13)));

			NLOG(niceName, " > Received error of projector " << c->id);
		}
	}
}

String PJLinkModule::convertError(String numError)
{
	std::string stringError;

	if (numError == "0") { stringError = "No error or no error detecting function"; }
	else if (numError == "1") { stringError = "**** Warning ****"; }
	else if (numError == "2") { stringError = "**** Error ****"; }

	return stringError;
}

void PJLinkModule::requestInputName(int id)
{
	if (indexInput <= inputListVp.size() - 1) sendMessageToClient("%2INNM ?" + inputListVp[indexInput], idVpInput);
	if (indexInput == inputListVp.size()) sendMessageToClient("%1INPT ?", id);
}

var PJLinkModule::getJSONData()
{
	var data = StreamingModule::getJSONData();

	var clientNames;
	for (auto& c : clients) clientNames.append(c->paramsCC.niceName);
	data.getDynamicObject()->setProperty("clientNames", clientNames);
	return data;
}

void PJLinkModule::loadJSONDataInternal(var data)
{
	ghostClientNames = data.getProperty("clientNames", var());
	updateClientsSetup();
	//while (clients.size() > 0) removeClient(clients[0]);
	
	StreamingModule::loadJSONDataInternal(data);
	ghostClientNames = var();
}

PJLinkModule::PJLinkClient::PJLinkClient(PJLinkModule* m, int id) :
	Thread("PJLink Client " + String(id)),
	pjlinkModule(m),
	id(id),
	passBytes(0),
	paramsCC("Projector " + String(id)),
	valuesCC("Projector " + String(id)),
	infosCC("Informations"),
	assigningFromRemote(false)
{
	paramsCC.nameCanBeChangedByUser = true;
	paramsCC.addControllableContainerListener(this);

	valuesCC.addChildControllableContainer(&infosCC);


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

	inputStatus = valuesCC.addEnumParameter("Input", "Input selected");												// input list and the selected one
	inputStatus->addOption("Not set", -1);
	inputResolutionInfo = valuesCC.addStringParameter("Input resolution", "Resolution of the signal", "");			// input resolution
	inputResolutionInfo->setControllableFeedbackOnly(true);
	updateInput = valuesCC.addTrigger("Update input resolution", "to update the input resolution");		// update input resolution

	//  Informations
	updateInfo = infosCC.addTrigger("Update informations", "update informations");
	brendInfo = infosCC.addStringParameter("Brend", "Brend of the projector", "");									// brend name
	brendInfo->setControllableFeedbackOnly(true);
	productNameInfo = infosCC.addStringParameter("Product name", "Product name of the projector", "");				// Product name
	productNameInfo->setControllableFeedbackOnly(true);
	displayNameInfo = infosCC.addStringParameter("display name", "display name of the projector", "");				// display name of the projector
	displayNameInfo->setControllableFeedbackOnly(true);
	resolutionRecommendedInfo = infosCC.addStringParameter("resolution recommended", "Resolution recommended", "");	// input resolution
	resolutionRecommendedInfo->setControllableFeedbackOnly(true);
	softVerInfo = infosCC.addStringParameter("Firmware", "firmware version", "");									// firmware version
	softVerInfo->setControllableFeedbackOnly(true);
	lampHourInfo = infosCC.addStringParameter("Lamp(s) hours", "total hours of lamp(s) operation", "");										// lamp hours
	lampHourInfo->setControllableFeedbackOnly(true);
	lampModelInfo = infosCC.addStringParameter("Lamp model", "Lamp replacement model number", "");					// Lamp replacement model number
	lampModelInfo->setControllableFeedbackOnly(true);
	filterUsageInfo = infosCC.addStringParameter("Filter usage", "Filter usage", "");								// filter usage time
	filterUsageInfo->setControllableFeedbackOnly(true);
	filterModelInfo = infosCC.addStringParameter("Filter Model", "Filter replacement model number", "");			// Filter replacement model number
	filterModelInfo->setControllableFeedbackOnly(true);
	fanErrorInfo = infosCC.addStringParameter("Fan error", "Fan error", "");											// Fan Error
	fanErrorInfo->setControllableFeedbackOnly(true);
	lampErrorInfo = infosCC.addStringParameter("Lamp error", "Lamp error", "");										// Lamp error
	lampErrorInfo->setControllableFeedbackOnly(true);
	temperatureErrorInfo = infosCC.addStringParameter("Temperature error", "Temperature error", "");				// Temperature error
	temperatureErrorInfo->setControllableFeedbackOnly(true);
	coverErrorInfo = infosCC.addStringParameter("Cover error", "Cover error", "");									// Cover error
	coverErrorInfo->setControllableFeedbackOnly(true);
	filterErrorInfo = infosCC.addStringParameter("Filter error", "Filter error", "");								// Filter error
	filterErrorInfo->setControllableFeedbackOnly(true);
	otherErrorInfo = infosCC.addStringParameter("Other error", "Other error", "");									// Other error
	otherErrorInfo->setControllableFeedbackOnly(true);
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

void PJLinkModule::PJLinkClient::controllableContainerNameChanged(ControllableContainer* cc)
{
	if (cc == &paramsCC)
	{
		valuesCC.setNiceName(paramsCC.niceName);
	}
}
