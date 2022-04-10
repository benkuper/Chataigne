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

class MQTTClientModule :
	public Module
#if JUCE_WINDOWS
	, public mosqpp::mosquittopp
#endif
	, public Thread
{
public:
	MQTTClientModule(const String& name = "MQTT Client", bool canHaveInput = true, bool canHaveOutput = true);
	virtual ~MQTTClientModule();

	
	enum Protocol { RAW, JSON };
	EnumParameter* protocol;

	StringParameter* host;
	IntParameter* port;
	IntParameter* keepAlive;
	BoolParameter* isConnected;

	Array<int> topicMap;
	ControllableContainer topicsCC;

	void clearItem() override;

	void onContainerParameterChangedInternal(Parameter* p) override;
	void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;
	void childStructureChanged(ControllableContainer* cc) override;

	void topicsCreateCallback(ControllableContainer* cc);

	void publishMessage(const String& topic, const String& message);

	void updateTopicSubs();

	void afterLoadJSONDataInternal() override;

	void run() override;

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