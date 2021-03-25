/*
  ==============================================================================

    CustomOSCCommand.h
    Created: 3 Nov 2016 12:41:23pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class CustomOSCCommand :
	public OSCCommand
{
public:
	CustomOSCCommand(CustomOSCModule * module, CommandContext context, var params, Multiplex * multiplex = nullptr);
	~CustomOSCCommand();
	
	var lastValue;

	CustomOSCModule* customOSCModule;

	CustomValuesCommandArgumentManager wildcardsContainer;
	Array<Parameter *> wildCardParams;

	void triggerInternal(int multiplexIndex) override;

	virtual void onContainerParameterChanged(Parameter * p) override;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	static CustomOSCCommand * create(ControllableContainer * module, CommandContext context, var params, Multiplex * multiplex) { return new CustomOSCCommand((CustomOSCModule *)module, context, params, multiplex); }


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomOSCCommand)
};