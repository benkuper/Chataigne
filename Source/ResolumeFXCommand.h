/*
  ==============================================================================

    ResolumeFXCommand.h
    Created: 19 Feb 2017 1:42:30pm
    Author:  Ben

  ==============================================================================
*/

#ifndef RESOLUMEFXCOMMAND_H_INCLUDED
#define RESOLUMEFXCOMMAND_H_INCLUDED


#include "ResolumeBaseCommand.h"

class ResolumeFXCommand :
	public ResolumeBaseCommand
{
public:
	ResolumeFXCommand(ResolumeModule * _module, CommandContext context, var params);
	~ResolumeFXCommand();

	String fxType;
	FloatParameter * valueParam;

	EnumParameter * nameParam; //for transform and audio
	IntParameter * fxIndexParam; //for fx
	IntParameter * indexParam; //for source and fx

	void rebuildAddress() override;
	void onContainerParameterChanged(Parameter * p) override;
	
	static BaseCommand * create(ControllableContainer * module, CommandContext context, var params) { 
		params.getDynamicObject()->setProperty("multiLevel", true);
		return new ResolumeFXCommand((ResolumeModule *)module, context, params);
	}

};




#endif  // RESOLUMEFXCOMMAND_H_INCLUDED
