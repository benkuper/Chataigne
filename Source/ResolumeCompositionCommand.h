/*
  ==============================================================================

    ResolumeCompositionCommand.h
    Created: 3 Nov 2016 12:33:59pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef RESOLUMECOMPOSITIONCOMMAND_H_INCLUDED
#define RESOLUMECOMPOSITIONCOMMAND_H_INCLUDED

#include "OSCCommand.h"
#include "ResolumeOutput.h"

class ResolumeCompositionCommand:
	public OSCCommand
{
public:
	ResolumeCompositionCommand(ResolumeOutput * output, CommandContext context, var params);
	~ResolumeCompositionCommand();


	static ResolumeCompositionCommand * create(ControllableContainer * output, CommandContext context, var params) { return new ResolumeCompositionCommand((ResolumeOutput *)output, context, params); }
};


#endif  // RESOLUMECOMPOSITIONCOMMAND_H_INCLUDED
