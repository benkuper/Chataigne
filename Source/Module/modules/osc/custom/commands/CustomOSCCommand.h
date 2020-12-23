/*
  ==============================================================================

    CustomOSCCommand.h
    Created: 3 Nov 2016 12:41:23pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../../commands/OSCCommand.h"
#include "../CustomOSCModule.h"
#include "Module/modules/common/commands/customvalues/CustomValuesCommandArgumentManager.h"

class CustomOSCCommand :
	public OSCCommand
{
public:
	CustomOSCCommand(OSCModule * module, CommandContext context, var params, IteratorProcessor * iterator = nullptr);
	~CustomOSCCommand();
	
	var lastValue;

	CustomValuesCommandArgumentManager wildcardsContainer;
	Array<Parameter *> wildCardParams;

	void triggerInternal(int iterationIndex) override;

	virtual void onContainerParameterChanged(Parameter * p) override;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	static CustomOSCCommand * create(ControllableContainer * module, CommandContext context, var params, IteratorProcessor * iterator) { return new CustomOSCCommand((CustomOSCModule *)module, context, params, iterator); }


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomOSCCommand)
};