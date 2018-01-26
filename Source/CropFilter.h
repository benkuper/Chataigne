/*
  ==============================================================================

    CropFilter.h
    Created: 26 Jan 2018 11:39:14am
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "MappingFilter.h"

class CropFilter :
	public MappingFilter
{
public:
	CropFilter(var params);
	~CropFilter();

	FloatParameter * targetMin;
	FloatParameter * targetMax;

	void processInternal() override;

	void filterParamChanged(Parameter *) override;

	Parameter * setupParameterInternal(Parameter * source) override;

	static CropFilter * create(var params) { return new CropFilter(params); }
	String getTypeString() const override { return "Crop"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CropFilter)
};
