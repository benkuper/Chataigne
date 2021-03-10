/*
  ==============================================================================

    ResolumeFXCommand.h
    Created: 19 Feb 2017 1:42:30pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

class ResolumeFXCommand :
	public ResolumeBaseCommand
{
public:
	ResolumeFXCommand(ResolumeModule * _module, CommandContext context, var params, Multiplex * multiplex = nullptr);
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
	String getTargetAddress(int multiplexIndex = 0) override;
	void onContainerParameterChanged(Parameter * p) override;

	void setupValueParam();
	
	static BaseCommand * create(ControllableContainer * module, CommandContext context, var params, Multiplex * multiplex) { 
		params.getDynamicObject()->setProperty("multiLevel", true);
		params.getDynamicObject()->setProperty("restrictToFX", true);
		return new ResolumeFXCommand((ResolumeModule *)module, context, params, multiplex);
	}

};