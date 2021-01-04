/*
  ==============================================================================

    OSCCommand.h
    Created: 3 Nov 2016 10:47:46am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Common/Command/BaseCommand.h"

class OSCModule;

class OSCCommand :
	public BaseCommand
{
public:
	OSCCommand(OSCModule * _module, CommandContext context, var params, Multiplex * multiplex = nullptr);
	virtual ~OSCCommand();

	OSCModule * oscModule;
	StringParameter * address;
	ControllableContainer argumentsContainer;

	String addressModel;
	bool rebuildAddressOnParamChanged;

	virtual void rebuildAddress();
	virtual String getTargetAddress(int multiplexIndex = 0);
	virtual String getTargetAddressInternal(const String& targetAddress, int multiplexIndex = 0) { return targetAddress; }

	void buildArgsAndParamsFromData(var data);

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	void controllableAdded(Controllable* c) override;

	void onContainerParameterChanged(Parameter * p) override;

	void triggerInternal(int multiplexIndex) override;

	static BaseCommand * create(ControllableContainer * module, CommandContext context, var params, Multiplex * multiplex) { return new OSCCommand((OSCModule *)module, context, params, multiplex); }


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OSCCommand)
};