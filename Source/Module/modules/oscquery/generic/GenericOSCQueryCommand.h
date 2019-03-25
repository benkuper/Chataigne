/*
  ==============================================================================

    GenericOSCQueryCommand.h
    Created: 28 Feb 2019 10:33:50pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Common/Command/BaseCommand.h"
#include "GenericOSCQueryModule.h"

class GenericOSCQueryCommand :
	public BaseCommand
{
public:
	GenericOSCQueryCommand(GenericOSCQueryModule * module, CommandContext context, var params);
	~GenericOSCQueryCommand();

	GenericOSCQueryModule * oscQueryModule;

	TargetParameter * target;
	WeakReference<Parameter> valueParam;

	void setupParamFromTarget();

	void onContainerParameterChanged(Parameter *) override;

	void triggerInternal() override;

	static GenericOSCQueryCommand * create(ControllableContainer * module, CommandContext context, var params) { return new GenericOSCQueryCommand((GenericOSCQueryModule *)module, context, params); }
};