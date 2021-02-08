/*
  ==============================================================================

    MappingOutput.h
    Created: 28 Oct 2016 8:12:01pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Common/Command/BaseCommandHandler.h"

class MappingOutput :
	public BaseCommandHandler
{
public:
	MappingOutput(Multiplex * multiplex = nullptr);
	virtual ~MappingOutput();

	Array<Array<WeakReference<Parameter>>> outParams;

	virtual void setOutParams(Array<WeakReference<Parameter>> outParams, int multiplexIndex);
	void updateCommandOutParams();

	virtual void setCommand(CommandDefinition * cd) override;

	void setValue(var value, int multiplexIndex);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MappingOutput)
};