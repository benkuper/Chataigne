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
	SimpleRemapFilter(var params);
	~SimpleRemapFilter();

	BoolParameter * useCustomInputRange;
	Point2DParameter * targetIn;
	Point2DParameter * targetOut;

	Parameter * setupSingleParameterInternal(Parameter * source) override;
	void processSingleParameterInternal(Parameter * source, Parameter * out) override;

	void filterParamChanged(Parameter *) override;

	static SimpleRemapFilter * create(var params) { return new SimpleRemapFilter(params); }
	String getTypeString() const override { return "Remap"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleRemapFilter)
};