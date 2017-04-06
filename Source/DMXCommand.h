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
	DMXCommand(DMXModule * _module, CommandContext context, var params);
	~DMXCommand();

	DMXModule * dmxModule;

	IntParameter * channel;
	IntParameter * value;

	void trigger() override;

	static BaseCommand * create(ControllableContainer * module, CommandContext context, var params) { return new DMXCommand((DMXModule *)module, context, params); }

	
};



#endif  // DMXCOMMAND_H_INCLUDED
