/*
  ==============================================================================

    CropFilter.h
    Created: 26 Jan 2018 11:39:14am
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../../MappingFilter.h"

class CropFilter :
	public MappingFilter
{
public:
	CropFilter(var params, Multiplex * multiplex);
	~CropFilter();

	FloatParameter * targetMin;
	FloatParameter * targetMax;

	Parameter* setupSingleParameterInternal(Parameter* source, int multiplexIndex) override;
	ProcessResult processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex) override;

	void filterParamChanged(Parameter *) override;

	String getTypeString() const override { return "Crop"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CropFilter)
};