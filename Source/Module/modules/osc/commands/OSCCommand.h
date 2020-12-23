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
	OSCCommand(OSCModule * _module, CommandContext context, var params, IteratorProcessor * iterator = nullptr);
	virtual ~OSCCommand();

	OSCModule * oscModule;
	StringParameter * address;
	ControllableContainer argumentsContainer;

	String addressModel;
	bool rebuildAddressOnParamChanged;

	virtual void rebuildAddress();
	virtual void rebuildAddressInternal(String& targetAddress) {}

	void buildArgsAndParamsFromData(var data);


	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	void onContainerParameterChanged(Parameter * p) override;

	void triggerInternal(int iterationIndex) override;

	static BaseCommand * create(ControllableContainer * module, CommandContext context, var params, IteratorProcessor * iterator) { return new OSCCommand((OSCModule *)module, context, params, iterator); }


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OSCCommand)
};