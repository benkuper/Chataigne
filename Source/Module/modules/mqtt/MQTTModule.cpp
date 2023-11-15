/*
  ==============================================================================

	MQTTClientModule.cpp
	Created: 10 Apr 2022 3:11:05pm
	Author:  bkupe

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

MQTTClientModule::MQTTClientModule(const String& name, bool canHaveInput, bool canHaveOutput) :
	Module(name),
	Thread("MQTT"),
#if JUCE_WINDOWS
	mosquittopp("Chataigne"),
#endif
	authenticationCC("Authentication"),
	topicsManager("Topics")
{

#if JUCE_WINDOWS
	mosqpp::lib_init();
	threaded_set(true);
#else
	NLOGWARNING(niceName, "MQTT is only supported on windows right now.");
#endif

	protocol = moduleParams.addEnumParameter("Default Protocol", "How to parse the incoming data");
	protocol->addOption("JSON", MQTTTopic::JSON)->addOption("Raw", MQTTTopic::RAW);

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

	moduleParams.addChildControllableContainer(&topicsManager);
	topicsManager.selectItemWhenCreated = false;
	topicsManager.addBaseManagerListener(this);

	includeValuesInSave = true;
	valuesCC.saveAndLoadRecursiveData = true;

	defManager->add(CommandDefinition::createDef(this, "", "Publish", &MQTTCommand::create));

	setupIOConfiguration(true, true);
	if (!Engine::mainEngine->isLoadingFile) startThread();
}

MQTTClientModule::~MQTTClientModule()
{
	topicsManager.removeBaseManagerListener(this);
	stopClient();
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
		else
		{
			stopClient();
		}
	}
}

void MQTTClientModule::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	Module::onControllableFeedbackUpdateInternal(cc, c);

	if (!isCurrentlyLoadingData)
	{
		if (c == host || c == port || c == keepAlive || c == authenticationCC.enabled || c == username || c == pass /* || c == useTLS*/)
		{
			stopClient();
			if (enabled->boolValue()) startThread();
		}

		if (dynamic_cast<MQTTTopic*>(c->parentContainer.get()) != nullptr || c == protocol)
		{
			updateTopicSubs();
		}
	}

	if (c == clearValues)
	{
		for (auto& cc : valuesCC.controllableContainers) cc->clear();
	}
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

void MQTTClientModule::itemAdded(MQTTTopic* item)
{
	if (!isCurrentlyLoadingData) updateTopicSubs();
}

void MQTTClientModule::itemsAdded(Array<MQTTTopic*> items)
{
	if (!isCurrentlyLoadingData) updateTopicSubs();
}

void MQTTClientModule::itemRemoved(MQTTTopic* item)
{
	if (isCurrentlyLoadingData) return;

#if JUCE_WINDOWS
	unsubscribe(&item->mid, item->topic->stringValue().toStdString().c_str());
#endif
	updateTopicSubs();
}

void MQTTClientModule::itemsRemoved(Array<MQTTTopic*> items)
{
	if (isCurrentlyLoadingData) return;

#if JUCE_WINDOWS
	for (auto& item : items) unsubscribe(&item->mid, item->topic->stringValue().toStdString().c_str());
#endif
	updateTopicSubs();
}

void MQTTClientModule::updateTopicSubs()
{
	GenericScopedLock lock(updateTopicLock);

	//valuesCC.clear();
	topicItemMap.clear();


	for (auto& topic : topicsManager.items)
	{
		String s = topic->topic->stringValue();
		if (s.isEmpty()) continue;

		MQTTTopic::Protocol p = topic->protocol->getValueDataAsEnum<MQTTTopic::Protocol>();
		if (p == MQTTTopic::DEFAULT) p = protocol->getValueDataAsEnum<MQTTTopic::Protocol>();

		switch (p)
		{
		case MQTTTopic::JSON:
		{
			//cleanup
			if (Parameter* p = valuesCC.getParameterByName(s, true)) valuesCC.removeControllable(p);

			ControllableContainer* cc = valuesCC.getControllableContainerByName(s, true);
			if (cc == nullptr)
			{
				cc = new ControllableContainer(s);
				cc->userCanAddControllables = true;
				cc->saveAndLoadRecursiveData = true;
				cc->saveAndLoadName = true;
				valuesCC.addChildControllableContainer(cc, true);
			}
		}
		break;

		case MQTTTopic::RAW:
		{
			//cleanup
			if (ControllableContainer* cc = valuesCC.getControllableContainerByName(s, true)) valuesCC.removeChildControllableContainer(cc);

			StringParameter* b = dynamic_cast<StringParameter*>(valuesCC.getParameterByName(s, true));
			if (b == nullptr) b = valuesCC.addStringParameter(s, "Last received message for this topic", "");
		}
		break;
		}

#if JUCE_WINDOWS
		subscribe(&topic->mid, s.toStdString().c_str());
#endif
		topicItemMap.set(s, topic);
	}

	Array<Controllable*> controllablesToRemove;
	for (auto& c : valuesCC.controllables)
	{
		bool found = false;
		for (auto& top : topicsManager.items)
		{
			if (top->topic->stringValue() == c->niceName)
			{
				found = true;
				break;
			}
		}
		if (!found) controllablesToRemove.add(c);
	}

	Array<ControllableContainer*> containersToRemove;
	for (auto& cc : valuesCC.controllableContainers)
	{
		bool found = false;
		for (auto& top : topicsManager.items)
		{
			if (top->topic->stringValue() == cc->niceName)
			{
				found = true;
				break;
			}
		}
		if (!found) containersToRemove.add(cc);
	}

	for (auto& c : controllablesToRemove) valuesCC.removeControllable(c);
	for (auto& cc : containersToRemove) valuesCC.removeChildControllableContainer(cc);

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
		isConnected->setValue(true);
	}
	else
	{
		NLOGERROR(niceName, "Connection error (" << result << ")");
		return;
	}


	loop_forever();


	while (!threadShouldExit())
	{
		//int rc = -1;

		//{
		//	rc = loop();
		//}

		//if (rc)
		//{
		//	LOG("Disconnected, reconnect");
		//	int rr = reconnect();
		//	LOG("Reconnection : " << rr);
		//}

		wait(2);
	}

	loop_stop();

	isConnected->setValue(false);
	disconnect();
#endif
}

void MQTTClientModule::stopClient()
{
#if JUCE_WINDOWS
	loop_stop();
	disconnect();
#endif
	stopThread(1000);
}

#if JUCE_WINDOWS
void MQTTClientModule::on_connect(int rc)
{
	//LOG("MQTT Connected : " << rc);
	//DBG("MQTT Connected event reveiced " << rc);
	isConnected->setValue(rc == 0);

	if (rc != 0) return;

	GenericScopedLock lock(updateTopicLock);

	//Subscribe
	for (auto& t : topicsManager.items)
	{
		String topic = t->topic->stringValue();
		if (topic.isEmpty()) continue;
		subscribe(&t->mid, topic.toStdString().c_str());
	}
}

void MQTTClientModule::on_disconnect(int rc)
{
	if (rc != 14) LOG("MQTT Disconnected : " << rc); //14 is loop disconnection, autoreconnect
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

	GenericScopedLock lock(updateTopicLock);

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

	if (!topicItemMap.contains(topic))
	{
		NLOGWARNING(niceName, "Received message from unknown topic " << topic);
		return;
	}

	MQTTTopic* topicItem = topicItemMap[topic];
	if (topicItem == nullptr) return;


	if (!topicItem->enabled->boolValue()) return;

	MQTTTopic::Protocol p = topicItem->protocol->getValueDataAsEnum<MQTTTopic::Protocol>();
	if (p == MQTTTopic::DEFAULT) p = protocol->getValueDataAsEnum<MQTTTopic::Protocol>();

	switch (p)
	{
	case MQTTTopic::JSON:
	{
		if (ControllableContainer* cc = valuesCC.getControllableContainerByName(topic, true))
		{
			var jsonData = JSON::parse(data);
			ControllableParser::createControllablesFromJSONObject(jsonData, cc);
		}
	}
	break;

	case MQTTTopic::RAW:
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

MQTTTopic::MQTTTopic(var params) :
	BaseItem(getTypeString()),
	mid(-1)
{
	topic = addStringParameter("Topic", "Topic to subscribe to", "");
	protocol = addEnumParameter("Protocol", "How to parse the incoming data");
	protocol->addOption("Default", DEFAULT)->addOption("JSON", JSON)->addOption("Raw", RAW);
}

MQTTTopic::~MQTTTopic()
{
}

//InspectableEditor* MQTTTopic::getEditorInternal(bool isRoot, Array<Inspectable*> inspectables)
//{
//	return new MQTTTopicEditor(this, isRoot);
//}
