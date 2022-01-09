/*
  ==============================================================================

    PJLinkModule.cpp
    Created: 5 Jan 2018 3:41:58pm
    Author:  Ben

  ==============================================================================
*/

PJLinkModule::PJLinkModule() :
	TCPClientModule(getDefaultTypeString(), 4352),
	passBytes(0)
{
	autoReconnect = false;

	alwaysShowValues = true;
	includeValuesInSave = true;

	setAutoAddAvailable(false);
	//autoAdd->setValue(false);

	streamingType->setValueWithData(LINES);

	password = moduleParams.addStringParameter("Password", "Password for the PJLink, leave empty if no pass set up on the Projector", "");
	
	powerStatus = valuesCC.addEnumParameter("Power Status", "Status of the projector");
	powerStatus->addOption("Not set", -1)->addOption("Powering OFF", "0")->addOption("Powering ON", "1")->addOption("Cooling", "2")->addOption("Warming Up", "3")->addOption("Unavailable Time", "ERR3")->addOption("Projector / Display Failure", "ERR4");
	
	shutterVideoStatus = valuesCC.addBoolParameter("Shutter Video Status", "Is the projector's shutter on", false);
	shutterAudioStatus = valuesCC.addBoolParameter("Shutter Audio Status", "Is the projector's shutter on", false);

	defManager->add(getBasePJCommand("", "Power On", "%1POWR 1", CommandContext::BOTH));
	defManager->add(getBasePJCommand("", "Power Off", "%1POWR 0", CommandContext::BOTH));
	defManager->add(getBasePJCommand("", "Power Status Request", "%1POWR ?", CommandContext::ACTION));
	defManager->add(getBasePJCommand("", "Shutter Video On", "%1AVMT 11", CommandContext::BOTH));
	defManager->add(getBasePJCommand("", "Shutter Video Off", "%1AVMT 10", CommandContext::BOTH));
	defManager->add(getBasePJCommand("", "Shutter Audio On", "%1AVMT 21", CommandContext::BOTH));
	defManager->add(getBasePJCommand("", "Shutter Audio Off", "%1AVMT 20", CommandContext::BOTH));
	defManager->add(getBasePJCommand("", "Shutter Video and Audio On", "%1AVMT 31", CommandContext::BOTH));
	defManager->add(getBasePJCommand("", "Shutter Video and Audio Off", "%1AVMT 30", CommandContext::BOTH));
	defManager->add(getBasePJCommand("", "Shutter Status Request", "%1AVMT ?", CommandContext::ACTION));

}

void PJLinkModule::sendMessageInternal(const String& message, var params)
{
	String encodedPass = juce::MD5((passBytes + password->stringValue()).toUTF8()).toHexString();
	String encodedMessage = password->stringValue().isEmpty() ? message : (encodedPass + message);
	
	if (!isThreadRunning() || !sender.isConnected())
	{
		if (!isThreadRunning()) stopThread(1000);
		startThread();
	}
	while (isThreadRunning() && !sender.isConnected()) {} // wait for the thread
	if (sender.isConnected()) TCPClientModule::sendMessageInternal(encodedMessage, params);
	else LOGERROR("Could not send message : " << message);
}

CommandDefinition * PJLinkModule::getBasePJCommand(const String & menu, const String & commandName, const String & command, CommandContext context)
{
	return CommandDefinition::createDef(this, menu, commandName, &SendStreamStringCommand::create, context)->addParam("fixedValue", command)->addParam("forceCR", true)->addParam("forceNL", false);
}


void PJLinkModule::initThread()
{
	TCPClientModule::initThread();
//	if (sender.isConnected()) timeAtConnect = Time::getMillisecondCounter() / 1000.0f;
//	else signalThreadShouldExit();
}

void PJLinkModule::runInternal()
{
	TCPClientModule::runInternal();
//	if (Time::getMillisecondCounter() / 1000.0f > timeAtConnect + 1)
//	{
//		sender.close();
//		signalThreadShouldExit();
//	}
}

void PJLinkModule::processDataLineInternal(const String & message)
{ 
	if (message.contains("PJLINK"))
	{
		StringArray mSplit;
		mSplit.addTokens(message, true);
		if (mSplit[1] == "1")
		{
			if (mSplit.size() >= 3)
			{
				passBytes = mSplit[2];
				NLOG(niceName, "Received PJLINK handshake with password gen key " << mSplit[2]);
			}
		}
		else if (mSplit[1] == "0")
		{
			NLOG(niceName, "Received PJLINK handshake, projector is not password protected");
		}
		else if (mSplit[1].contains("ERR"))
		{
			NLOGERROR(niceName, "PJLINK Authentication error, please verify password !");
		}
	}else if (message.contains("%1POWR="))
	{
		String status = message.substring(7);
		if (status != "OK")
		{
			if (logIncomingData->boolValue()) NLOG(niceName, "Received Power status " << status);
			powerStatus->setValueWithData(status);
		}
		else
		{
			if (logIncomingData->boolValue()) NLOG(niceName, "Project power command accepted !");
		}
	}else if (message.contains("%1AVMT="))
	{
		bool isVideo = message[7] == '1' || message[7] == '3';
		bool isAudio = message[7] == '2' || message[7] == '3';
		bool isOn = message[8] == '1';

		if (isVideo)
		{
			if (logIncomingData->boolValue()) NLOG(niceName, "Received Video shutter status " << (isOn?"ON":"OFF"));
			shutterVideoStatus->setValue(isOn);
		}

		if (isAudio)
		{
			if (logIncomingData->boolValue()) NLOG(niceName, "Received Video shutter status " << (isOn ? "ON" : "OFF"));
			shutterAudioStatus->setValue(isOn);
		}
	}
}
