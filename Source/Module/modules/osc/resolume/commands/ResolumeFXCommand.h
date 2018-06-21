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
	
	static BaseCommand * create(ControllableContainer * module, CommandContext context, var params) { 
		params.getDynamicObject()->setProperty("multiLevel", true);
		return new ResolumeFXCommand((ResolumeModule *)module, context, params);
	}

};




#endif  // RESOLUMEFXCOMMAND_H_INCLUDED
