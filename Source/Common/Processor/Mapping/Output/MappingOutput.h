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
	MappingOutput();
	virtual ~MappingOutput();

	Controllable::Type outputType;
	virtual void setOutputType(Controllable::Type type);
	virtual void setCommand(CommandDefinition * cd) override;

	void setValue(var value);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MappingOutput)
};