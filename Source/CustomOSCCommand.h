/*
  ==============================================================================

    CustomOSCCommand.h
    Created: 3 Nov 2016 12:41:23pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef CUSTOMOSCCOMMAND_H_INCLUDED
#define CUSTOMOSCCOMMAND_H_INCLUDED

#include "OSCCommand.h"
#include "CustomOSCModule.h"
#include "CustomOSCCommandArgumentManager.h"

class CustomOSCCommand :
	public OSCCommand,
	public CustomOSCCommandArgumentManager::ManagerListener
{
public:
	CustomOSCCommand(CustomOSCModule * output, CommandContext context, var params);
	~CustomOSCCommand();
	
	CustomOSCCommandArgumentManager argManager;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;


	void trigger() override;

	void useForMappingChanged(CustomOSCCommandArgument * a) override;

	static CustomOSCCommand * create(ControllableContainer * module, CommandContext context, var params) { return new CustomOSCCommand((CustomOSCModule *)module, context, params); }

private:
	WeakReference<CustomOSCCommand>::Master masterReference;
	friend class WeakReference<CustomOSCCommand>;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomOSCCommand)
};


#endif  // CUSTOMOSCCOMMAND_H_INCLUDED
