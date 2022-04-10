/*
  ==============================================================================

	MQTTCommands.cpp
	Created: 10 Apr 2022 7:24:03pm
	Author:  bkupe

  ==============================================================================
*/


MQTTCommand::MQTTCommand(MQTTClientModule* _module, CommandContext context, var params, Multiplex* multiplex) :
	BaseCommand(_module, context, params, multiplex),
	mqttModule(_module),
	payload(nullptr)
{

	topic = addStringParameter("Topic", "Topic to send to", "");
	payload = addStringParameter("Payload", "This data to send", "");
	payload->multiline = true;
}

MQTTCommand::~MQTTCommand()
{
}


void MQTTCommand::triggerInternal(int multiplexIndex)
{
	mqttModule->publishMessage(getLinkedValue(topic, multiplexIndex), getLinkedValue(payload, multiplexIndex));
}