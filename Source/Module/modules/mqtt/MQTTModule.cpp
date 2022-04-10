#include "MQTTModule.h"
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

	defManager->add(CommandDefinition::createDef(this, "", "Publish", &MQTTCommand::create));


	topicsCC.userCanAddControllables = true;
	topicsCC.customUserCreateControllableFunc = std::bind(&MQTTClientModule::topicsCreateCallback, this, std::placeholders::_1);
	moduleParams.addChildControllableContainer(&topicsCC);

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
		if (c == host || c == port || c == keepAlive)
		{
			stopThread(1000);
			if (enabled->boolValue()) startThread();
		}

		if (c->parentContainer == &topicsCC || c == protocol)
		{
			updateTopicSubs();
		}
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

	int mid = 0;
	int result = publish(&mid, topic.toStdString().c_str(), message.length(), message.toStdString().c_str());

	if (logOutgoingData->boolValue())
	{
		NLOG(niceName, "Sent topic (" << result << ") : " << topic << ", message : " << message);
	}
#endif
}

void MQTTClientModule::updateTopicSubs()
{
#if JUCE_WINDOWS
	for (int i = 0; i < topicsCC.controllables.size(); i++)
	{
		int mid = topicMap[i];
		unsubscribe(&mid, ((StringParameter*)topicsCC.controllables[i])->stringValue().toStdString().c_str());
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

	//Subscribe
	for (int i = 0; i < topicsCC.controllables.size(); i++)
	{
		int mid = topicMap[i];
		subscribe(&mid, ((StringParameter*)topicsCC.controllables[i])->stringValue().toStdString().c_str());
		DBG(mid << " <> " << topicMap[i]);
		topicMap.set(i, mid);
	}

	while (!threadShouldExit())
	{
		loop(1000, 10);
		wait(10);
	}

	isConnected->setValue(false);
	disconnect();
#endif
}

#if JUCE_WINDOWS
void MQTTClientModule::on_connect(int rc)
{
	DBG("MQTT Connected event reveiced");

}

void MQTTClientModule::on_disconnect(int rc)
{
	isConnected->setValue(false);
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
	if (logIncomingData->boolValue())
	{
		NLOG(niceName, "Received from topic " << topic << " : " << data);
	}

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
	int index = topicMap.indexOf(mid);
	if (index >= 0 && topicsCC.controllables.size() > index) NLOG(niceName, "Subscribed to " << ((Parameter*)topicsCC.controllables[index])->stringValue());
}

void MQTTClientModule::on_unsubscribe(int mid)
{
	int index = topicMap.indexOf(mid);
	if (index >= 0 && topicsCC.controllables.size() > index) NLOG(niceName, "Unsubscribed from " << ((Parameter*)topicsCC.controllables[index])->stringValue());
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