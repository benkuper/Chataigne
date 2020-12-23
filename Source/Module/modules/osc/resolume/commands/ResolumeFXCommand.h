/*
  ==============================================================================

    ResolumeFXCommand.h
    Created: 19 Feb 2017 1:42:30pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "ResolumeBaseCommand.h"

class ResolumeFXCommand :
	public ResolumeBaseCommand
{
public:
	ResolumeFXCommand(ResolumeModule * _module, CommandContext context, var params, IteratorProcessor * iterator = nullptr);
	~ResolumeFXCommand();

	String fxType;

	EnumParameter * nameParam; //for transform and audio
	IntParameter * fxIndexParam; //for fx

	Parameter * valueParam;
	
	//Resolume 5
	IntParameter * indexParam; //for source and fx

	//Resolume 6
	EnumParameter * fxParamType;
	StringParameter * fxName;
	StringParameter * fxParamName;

	void rebuildParametersInternal() override;
	void rebuildAddress() override;
	void onContainerParameterChanged(Parameter * p) override;

	void setupValueParam();
	
	static BaseCommand * create(ControllableContainer * module, CommandContext context, var params, IteratorProcessor * iterator) { 
		params.getDynamicObject()->setProperty("multiLevel", true);
		params.getDynamicObject()->setProperty("restrictToFX", true);
		return new ResolumeFXCommand((ResolumeModule *)module, context, params, iterator);
	}

};