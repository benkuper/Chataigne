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
#include "CustomOSCOutput.h"

class CustomOSCCommand :
	public OSCCommand
{
public:
	CustomOSCCommand(CustomOSCOutput * output, CommandContext context, var params);
	~CustomOSCCommand();

	static CustomOSCCommand * create(ControllableContainer * output, CommandContext context, var params) { return new CustomOSCCommand((CustomOSCOutput *)output, context, params); }
};


#endif  // CUSTOMOSCCOMMAND_H_INCLUDED
