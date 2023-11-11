/*
  ==============================================================================

	MQTTModule.h
	Created: 10 Apr 2022 3:11:05pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

#if JUCE_WINDOWS
#include <mosquittopp.h>
#endif


class MQTTTopic :
	public BaseItem
{
public:
	enum Protocol { RAW, JSON, DEFAULT };

	MQTTTopic(var params = var());
	virtual ~MQTTTopic();

	StringParameter* topic;
	EnumParameter* protocol;
	int mid;

	//InspectableEditor* getEditorInternal(bool isRoot, Array<Inspectable*> inspectables = Array<Inspectable*>()) override;

	DECLARE_TYPE("Topic");
};

class MQTTClientModule :
	public Module
#if JUCE_WINDOWS
	, public mosqpp::mosquittopp
#endif
	, public Thread
	, public BaseManager<MQTTTopic>::ManagerListener
{
public:
	MQTTClientModule(const String& name = "MQTT Client", bool canHaveInput = true, bool canHaveOutput = true);
	virtual ~MQTTClientModule();


	EnumParameter* protocol;

	StringParameter* host;
	IntParameter* port;


	IntParameter* keepAlive;
	BoolParameter* isConnected;
	Trigger* clearValues;


	EnablingControllableContainer authenticationCC;
	StringParameter* username;
	StringParameter* pass;
	//BoolParameter* useTLS;
	HashMap<String, MQTTTopic*> topicItemMap;

	SpinLock updateTopicLock;
	BaseManager<MQTTTopic> topicsManager;

	

	const Identifier dataEventId = "dataEvent";

	void clearItem() override;

	void onContainerParameterChangedInternal(Parameter* p) override;
	void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;

	void publishMessage(const String& topic, const String& message);

	void itemAdded(MQTTTopic* item) override;
	void itemsAdded(Array<MQTTTopic*> item) override;
	void itemRemoved(MQTTTopic* item) override;
	void itemsRemoved(Array<MQTTTopic*> item) override;

	void updateTopicSubs();

	void afterLoadJSONDataInternal() override;

	void run() override;

	void stopClient();

	//mosquitto
#if JUCE_WINDOWS
	void on_connect(int rc) override;
	virtual void on_connect_with_flags(int /*rc*/, int /*flags*/) override { return; }
	virtual void on_disconnect(int rc) override;
	virtual void on_publish(int mid) override;
	virtual void on_message(const struct mosquitto_message* message) override;
	virtual void on_subscribe(int mid, int qos_count, const int* granted_qos) override;
	virtual void on_unsubscribe(int mid) override;
	virtual void on_log(int level, const char* str) override;
	virtual void on_error() override;
#endif

	static MQTTClientModule* create() { return new MQTTClientModule(); }
	virtual String getDefaultTypeString() const override { return "MQTT Client"; }
};