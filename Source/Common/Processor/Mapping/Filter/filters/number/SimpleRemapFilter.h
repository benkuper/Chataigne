/*
  ==============================================================================

    SimpleRemapFilter.h
    Created: 4 Feb 2017 5:39:07pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../../MappingFilter.h"


class SimpleRemapFilter :
	public MappingFilter
{
public:
	SimpleRemapFilter(var params, Multiplex* multiplex);
	~SimpleRemapFilter();

	BoolParameter * useCustomInputRange;
	Point2DParameter * targetIn;
	Point2DParameter * targetOut;
	BoolParameter* forceFloatOutput;

	Parameter * setupSingleParameterInternal(Parameter * source, int multiplexIndex) override;
	ProcessResult processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex) override;

	void filterParamChanged(Parameter *) override;

	String getTypeString() const override { return "Remap"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleRemapFilter)
};