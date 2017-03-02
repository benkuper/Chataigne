/*
  ==============================================================================

    ReaperTimeCommand.h
    Created: 26 Jan 2017 2:42:39pm
    Author:  Ben

  ==============================================================================
*/

#ifndef REAPERTIMECOMMAND_H_INCLUDED
#define REAPERTIMECOMMAND_H_INCLUDED

#include "OSCCommand.h"
#include "ReaperModule.h"

class ReaperTimeCommand :
	public OSCCommand
{
public:
	ReaperTimeCommand(ReaperModule * _module, CommandContext context, var params);
	~ReaperTimeCommand();

	ReaperModule * reaperModule;
	FloatParameter * timeParam;

	static ReaperTimeCommand * create(ControllableContainer * module, CommandContext context, var params) { return new ReaperTimeCommand((ReaperModule *)module, context, params); }

};



#endif  // REAPERTIMECOMMAND_H_INCLUDED
