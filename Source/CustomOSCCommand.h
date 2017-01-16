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

class CustomOSCCommand :
	public OSCCommand
{
public:
	CustomOSCCommand(CustomOSCModule * output, CommandContext context, var params);
	~CustomOSCCommand();
	
	void addIntArgument();
	void addFloatArgument();
	void addStringArgument();
	void addArgument(Parameter *);

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	InspectableEditor * getEditor(bool /*isRoot*/) override;
	static CustomOSCCommand * create(ControllableContainer * module, CommandContext context, var params) { return new CustomOSCCommand((CustomOSCModule *)module, context, params); }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomOSCCommand)
};


#endif  // CUSTOMOSCCOMMAND_H_INCLUDED
