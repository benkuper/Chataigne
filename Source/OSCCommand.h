/*
  ==============================================================================

    OSCCommand.h
    Created: 3 Nov 2016 10:47:46am
    Author:  bkupe

  ==============================================================================
*/

#ifndef OSCCOMMAND_H_INCLUDED
#define OSCCOMMAND_H_INCLUDED

#include "OutputCommand.h"

class OSCOutput;

class OSCCommand :
	public OutputCommand
{
public:
	OSCCommand(OSCOutput * _output, CommandContext context, var params);
	virtual ~OSCCommand();

	OSCOutput * oscOutput;
	StringParameter * address;
	Array<Parameter *> arguments;

	void trigger() override;

	static BaseCommand * create(ControllableContainer * output, CommandContext context, var params) { return new OSCCommand((OSCOutput *)output, context, params); }
};


#endif  // OSCCOMMAND_H_INCLUDED
