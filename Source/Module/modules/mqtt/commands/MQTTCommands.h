/*
  ==============================================================================

    MQTTCommands.h
    Created: 10 Apr 2022 7:24:03pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once


class MQTTCommand :
	public BaseCommand
{
public:
	MQTTCommand(MQTTClientModule* _module, CommandContext context, var params, Multiplex* multiplex = nullptr);
	~MQTTCommand();

	MQTTClientModule* mqttModule;

	StringParameter* topic;
	StringParameter* payload;

	void triggerInternal(int multiplexIndex) override;

	static BaseCommand* create(ControllableContainer* module, CommandContext context, var params, Multiplex* multiplex) { return new MQTTCommand((MQTTClientModule*)module, context, params, multiplex); }
};
