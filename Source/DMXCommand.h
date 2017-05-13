/*
  ==============================================================================

    DMXCommand.h
    Created: 6 Apr 2017 10:22:35pm
    Author:  Ben

  ==============================================================================
*/

#ifndef DMXCOMMAND_H_INCLUDED
#define DMXCOMMAND_H_INCLUDED

#include "BaseCommand.h"
#include "DMXModule.h"

class DMXCommand :
	public BaseCommand
{
public:
	enum DMXAction { SET_VALUE, SET_RANGE, COLOR, CLEAR_ALL};

	DMXCommand(DMXModule * _module, CommandContext context, var params);
	~DMXCommand();

	DMXAction dmxAction;
	DMXModule * dmxModule;

	IntParameter * channel;
	IntParameter * channel2;
	IntParameter * value;
	ColorParameter * colorParam;

	void trigger() override;

	static BaseCommand * create(ControllableContainer * module, CommandContext context, var params) { return new DMXCommand((DMXModule *)module, context, params); }

	
};



#endif  // DMXCOMMAND_H_INCLUDED
