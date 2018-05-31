/*
  ==============================================================================

    CustomOSCCommand.h
    Created: 3 Nov 2016 12:41:23pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef CUSTOMOSCCOMMAND_H_INCLUDED
#define CUSTOMOSCCOMMAND_H_INCLUDED

#include "../../commands/OSCCommand.h"
#include "../CustomOSCModule.h"
#include "Module/modules/common/commands/customvalues/CustomValuesCommandArgumentManager.h"

class CustomOSCCommand :
	public OSCCommand,
	public CustomValuesCommandArgumentManager::ManagerListener
{
public:
	CustomOSCCommand(CustomOSCModule * output, CommandContext context, var params);
	~CustomOSCCommand();
	

	var lastValue;

	void trigger() override;
	void useForMappingChanged(CustomValuesCommandArgument * a) override;


	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	static CustomOSCCommand * create(ControllableContainer * module, CommandContext context, var params) { return new CustomOSCCommand((CustomOSCModule *)module, context, params); }


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomOSCCommand)
};


#endif  // CUSTOMOSCCOMMAND_H_INCLUDED
