/*
  ==============================================================================

    ResolumeClipCommand.h
    Created: 3 Nov 2016 12:34:21pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef RESOLUMECLIPCOMMAND_H_INCLUDED
#define RESOLUMECLIPCOMMAND_H_INCLUDED

#include "ResolumeBaseCommand.h"

class ResolumeClipCommand :
	public ResolumeBaseCommand
{
public:
	ResolumeClipCommand(ResolumeModule * _module, CommandContext context, var params);
	~ResolumeClipCommand();

	IntParameter * connectParam;

	//Multi
	IntParameter * firstClip;
	IntParameter * lastClip;
	BoolParameter * loopClips;
	BoolParameter * randomClips;
	Random clipRand;

	void onContainerParameterChanged(Parameter *) override;
	void triggerInternal() override;


	static BaseCommand * create(ControllableContainer * module, CommandContext context, var params) { 
		params.getDynamicObject()->setProperty("multiLevel", false);
		return new ResolumeClipCommand((ResolumeModule *)module, context, params); 
	}

};


#endif  // RESOLUMECLIPCOMMAND_H_INCLUDED
