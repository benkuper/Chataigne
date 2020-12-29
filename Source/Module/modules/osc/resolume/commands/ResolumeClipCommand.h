/*
  ==============================================================================

    ResolumeClipCommand.h
    Created: 3 Nov 2016 12:34:21pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "ResolumeBaseCommand.h"

class ResolumeClipCommand :
	public ResolumeBaseCommand
{
public:
	ResolumeClipCommand(ResolumeModule * _module, CommandContext context, var params, Multiplex * multiplex = nullptr);
	~ResolumeClipCommand();

	IntParameter * connectParam;

	//Multi
	IntParameter * firstClip;
	IntParameter * lastClip;
	BoolParameter * loopClips;
	BoolParameter * randomClips;
	Random clipRand;

	bool needsOnOff;

	void rebuildParametersInternal() override;

	void onContainerParameterChanged(Parameter *) override;
	void triggerInternal(int multiplexIndex) override;


	static BaseCommand * create(ControllableContainer * module, CommandContext context, var params, Multiplex * multiplex) { 
		params.getDynamicObject()->setProperty("multiLevel", false);
		return new ResolumeClipCommand((ResolumeModule *)module, context, params, multiplex); 
	}

};