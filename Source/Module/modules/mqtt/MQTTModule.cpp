/*
  ==============================================================================

	MQTTClientModule.cpp
	Created: 10 Apr 2022 3:11:05pm
	Author:  bkupe

  ==============================================================================
*/

MQTTClientModule::MQTTClientModule(const String& name, bool canHaveInput, bool canHaveOutput) :
	Module(name),
	Thread("MQTT"),
#if JUCE_WINDOWS
	mosquittopp("Chataigne"),
#endif
	authenticationCC("Authentication"),
	topicsCC("Topics")
{

#if JUCE_WINDOWS
	mosqpp::lib_init();
#else
	NLOGWARNING(niceName, "MQTT is only supported on windows right now.");
#endif

	protocol = moduleParams.addEnumParameter("Protocol", "How to parse the incoming data");
	protocol->addOption("JSON", JSON)->addOption("Raw", RAW);

	host = moduleParams.addStringParameter("Host", "The MQTT Broker's host address", "127.0.0.1");
	port = moduleParams.addIntParameter("Port", "The MQTT Broker's port", 1883, 1, 65535);
	keepAlive = moduleParams.addIntParameter("Keep Alive", "The time to keep alive the connection, in seconds", 60, 1);
	isConnected = moduleParams.addBoolParameter("Is Connected", "Is MQTT Connected ?", false);
	isConnected->setControllableFeedbackOnly(true);
	connectionFeedbackRef = isConnected;

	username = authenticationCC.addStringParameter("Username", "If using authentication, this is the username to use for the authentication", "");
	pass = authenticationCC.addStringParameter("Password", "If using authentication, this is the password to use for the authentication", "");
	//useTLS = authenticationCC.addBoolParameter("Use TLS", "If using authentication, this decides if using TLS or not", false);

	authenticationCC.enabled->setValue(false);
	moduleParams.addChildControllableContainer(&authenticationCC);

	topicsCC.userCanAddControllables = true;
	topicsCC.customUserCreateControllableFunc = std::bind(&MQTTClientModule::topicsCreateCallback, this, std::placeholders::_1);
	moduleParams.addChildControllableContainer(&topicsCC);

	includeValuesInSave = true;
	valuesCC.saveAndLoadRecursiveData = true;

	defManager->add(CommandDefinition::createDef(this, "", "Publish", &MQTTCommand::create));

	setupIOConfiguration(true, true);
	if (!Engine::mainEngine->isLoadingFile) startThread();
}

MQTTClientModule::~MQTTClientModule()
{
	stopThread(1000);
}

void MQTTClientModule::clearItem()
{
	Module::clearItem();

#if JUCE_WINDOWS
	mosqpp::lib_cleanup();
#endif
}


void MQTTClientModule::onContainerParameterChangedInternal(Parameter* p)
{
	Module::onContainerParameterChangedInternal(p);
	if (p == enabled)
	{
		if (enabled->boolValue()) startThread();
		else stopThread(1000);
	}
}

void MQTTClientModule::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	Module::onControllableFeedbackUpdateInternal(cc, c);

	if (!isCurrentlyLoadingData)
	{
		if (c == host || c == port || c == keepAlive || c == authenticationCC.enabled || c == username || c == pass /* || c == useTLS*/)
		{
			stopThread(1000);
			if (enabled->boolValue()) startThread();
		}

		if (c->parentContainer == &topicsCC || c == protocol)
		{
			updateTopicSubs();
		}
	}

	if (c == clearValues)
	{
		for (auto& cc : valuesCC.controllableContainers) cc->clear();
	}

}

void MQTTClientModule::childStructureChanged(ControllableContainer* cc)
{
	Module::childStructureChanged(cc);

	if (!isCurrentlyLoadingData)
	{
		if (cc == &moduleParams)
		{
			updateTopicSubs();
		}
	}
}

void MQTTClientModule::topicsCreateCallback(ControllableContainer* cc)
{
	StringParameter* p = topicsCC.addStringParameter("Topic 1", "Topic to subscribe to", "");
	p->saveValueOnly = false;
	p->isRemovableByUser = true;
	topicsCC.queuedNotifier.addMessage(new ContainerAsyncEvent(ContainerAsyncEvent::ControllableContainerNeedsRebuild, &topicsCC));
}

void MQTTClientModule::publishMessage(const String& topic, const String& message)
{
	if (!enabled->boolValue()) return;

#if JUCE_WINDOWS
	if (!isConnected->boolValue())
	{
		NLOGWARNING(niceName, "Not connected, not sending");
		return;
	}

	int result = publish(NULL, topic.toStdString().c_str(), message.length(), message.toStdString().c_str(), 2);

	if (logOutgoingData->boolValue())
	{
		NLOG(niceName, "Sent topic (" << result << ") : " << topic << ", message : " << message);
	}
#endif
}

void MQTTClientModule::updateTopicSubs(bool keepData)
{
	var oldData(new DynamicObject());

#if JUCE_WINDOWS
	for (int i = 0; i < topicsCC.controllables.size(); i++)
	{
		int mid = topicMap[i];
		String t = ((StringParameter*)topicsCC.controllables[i])->stringValue();
		unsubscribe(&mid, t.toStdString().c_str());

		if (ControllableContainer* cc = valuesCC.getControllableContainerByName(t))
		{
			oldData.getDynamicObject()->setProperty(t, cc->getJSONData());
		}
	}
#endif


	valuesCC.clear();
	topicMap.clear();

	for (auto& c : topicsCC.controllables)
	{
		String s = ((StringParameter*)c)->stringValue();
		if (s.isEmpty()) continue;

		Protocol p = protocol->getValueDataAsEnum<Protocol>();

		switch (p)
		{
		case JSON:
		{
			ControllableContainer* cc = new ControllableContainer(s);
			cc->userCanAddControllables = true;
			cc->saveAndLoadRecursiveData = true;
			cc->saveAndLoadName = true;
			if (oldData.hasProperty(s)) cc->loadJSONData(oldData.getDynamicObject()->getProperty(s));
			valuesCC.addChildControllableContainer(cc, true);
		}
		break;

		case RAW:
		{
			StringParameter* b = valuesCC.addStringParameter(s, "Last received message for this topic", "");
			b->isSavable = false;
		}
		break;
		}


		int mid = 0;
#if JUCE_WINDOWS
		subscribe(&mid, s.toStdString().c_str());
#endif
		topicMap.add(mid);
	}

	valuesCC.queuedNotifier.addMessage(new ContainerAsyncEvent(ContainerAsyncEvent::ControllableContainerNeedsRebuild, &valuesCC));
}

void MQTTClientModule::afterLoadJSONDataInternal()
{
	Module::afterLoadJSONDataInternal();
	updateTopicSubs();
	startThread();
}

void MQTTClientModule::run()
{
	wait(100);

#if JUCE_WINDOWS
	if (isConnected->boolValue())
	{
		isConnected->setValue(false);
		disconnect();
	}

	NLOG(niceName, "Connecting to " << host->stringValue() << ":" << port->intValue() << "...");

	if (authenticationCC.enabled->boolValue())
	{
		username_pw_set(username->stringValue().toStdString().c_str(), pass->stringValue().toStdString().c_str());
		//add tls here
	}
	else username_pw_set(NULL);

	int result = connect(host->stringValue().toStdString().c_str(), port->intValue(), keepAlive->intValue());


	if (result == 0)
	{
		NLOG(niceName, "Connected");
	}
	else
	{
		NLOGERROR(niceName, "Connection error (" << result << ")");
		return;
	}



	while (!threadShouldExit())
	{
		int rc = loop();
		if (rc)
		{
			//LOG("Disconnected, reconnect");
			reconnect();
			//LOG("Reconnection : " << rr);
		}
		//wait(2);
	}

	isConnected->setValue(false);
	disconnect();
#endif
}

#if JUCE_WINDOWS
void MQTTClientModule::on_connect(int rc)
{
	//LOG("MQTT Connected : " << rc);
	//DBG("MQTT Connected event reveiced " << rc);
	isConnected->setValue(rc == 0);

	if (rc != 0) return;

	//Subscribe
	for (int i = 0; i < topicsCC.controllables.size(); i++)
	{
		int mid = topicMap[i];
		String topic = ((StringParameter*)topicsCC.controllables[i])->stringValue();
		if (topic.isEmpty()) continue;
		subscribe(&mid, topic.toStdString().c_str());
		DBG(mid << " <> " << topicMap[i]);
		topicMap.set(i, mid);
	}
}

void MQTTClientModule::on_disconnect(int rc)
{
	if(rc != 14) LOG("MQTT Disconnected : " << rc); //14 is loop disconnection, autoreconnect
	isConnected->setValue(false);
	if (rc != 0) reconnect();
}

void MQTTClientModule::on_publish(int mid)
{
	outActivityTrigger->trigger();
}

void MQTTClientModule::on_message(const mosquitto_message* message)
{
	if (!enabled->boolValue()) return;

	String topic(message->topic);
	String data((char*)message->payload, message->payloadlen);
	Array<var> args;

	if (logIncomingData->boolValue())
	{
		NLOG(niceName, "Received from topic " << topic << " : " << data);
	}

	args.add(data);
	args.add(topic);
	scriptManager->callFunctionOnAllItems(dataEventId, args);

	inActivityTrigger->trigger();

	Protocol p = protocol->getValueDataAsEnum<Protocol>();
	switch (p)
	{
	case JSON:
	{
		if (ControllableContainer* cc = valuesCC.getControllableContainerByName(topic, true))
		{
			var jsonData = JSON::parse(data);
			ControllableParser::createControllablesFromJSONObject(jsonData, cc);
		}
	}
	break;

	case RAW:
	{
		if (Parameter* p = valuesCC.getParameterByName(topic, true))
		{
			((StringParameter*)p)->setValue(data);
		}
	}
	break;
	}
}

void MQTTClientModule::on_subscribe(int mid, int qos_count, const int* granted_qos)
{
	//int index = topicMap.indexOf(mid);
	//if (index >= 0 && topicsCC.controllables.size() > index) NLOG(niceName, "Subscribed to " << ((Parameter*)topicsCC.controllables[index])->stringValue());
}

void MQTTClientModule::on_unsubscribe(int mid)
{
	//int index = topicMap.indexOf(mid);
	//if (index >= 0 && topicsCC.controllables.size() > index) NLOG(niceName, "Unsubscribed from " << ((Parameter*)topicsCC.controllables[index])->stringValue());
}

void MQTTClientModule::on_log(int level, const char* str)
{
	DBG("Log (" << level << ") " << String(str));
}

void MQTTClientModule::on_error()
{
	NLOGERROR(niceName, "Error !");
}
#endif