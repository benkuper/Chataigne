/*
  ==============================================================================

    OSCCommand.h
    Created: 3 Nov 2016 10:47:46am
    Author:  bkupe

  ==============================================================================
*/

#ifndef OSCCOMMAND_H_INCLUDED
#define OSCCOMMAND_H_INCLUDED

#include "ModuleCommand.h"

class OSCModule;

class OSCCommand :
	public ModuleCommand
{
public:
	OSCCommand(OSCModule * _module, CommandContext context, var params);
	virtual ~OSCCommand();

	OSCModule * oscModule;
	StringParameter * address;
	ControllableContainer argumentsContainer;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	void trigger() override;

	static BaseCommand * create(ControllableContainer * module, CommandContext context, var params) { return new OSCCommand((OSCModule *)module, context, params); }


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OSCCommand)
};


#endif  // OSCCOMMAND_H_INCLUDED
