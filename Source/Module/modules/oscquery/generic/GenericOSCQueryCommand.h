/*
  ==============================================================================

    GenericOSCQueryCommand.h
    Created: 28 Feb 2019 10:33:50pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class GenericOSCQueryCommand :
	public BaseCommand
{
public:
	GenericOSCQueryCommand(GenericOSCQueryModule * module, CommandContext context, var params, Multiplex * multiplex = nullptr);
	~GenericOSCQueryCommand();

	GenericOSCQueryModule * oscQueryModule;

	TargetParameter * target;
	WeakReference<Parameter> valueParam;

	var valueGhostData; //when loading/reloading data

	void setupParamFromTarget();

	void onContainerParameterChanged(Parameter *) override;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	void triggerInternal(int multiplexIndex) override;

	static GenericOSCQueryCommand * create(ControllableContainer * module, CommandContext context, var params, Multiplex * multiplex) { return new GenericOSCQueryCommand((GenericOSCQueryModule *)module, context, params, multiplex); }
};