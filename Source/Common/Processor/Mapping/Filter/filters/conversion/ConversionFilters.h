/*
  ==============================================================================

    ConversionFilters.h
    Created: 17 Apr 2018 10:25:04am
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../../MappingFilter.h"

class ToIntFilter :
	public MappingFilter
{
public:
	ToIntFilter(var params);
	~ToIntFilter();

	enum Mode { FLOOR, ROUND, CEIL };
	EnumParameter * modeParam;

	void processInternal() override;

	static ToIntFilter * create(var params) { return new ToIntFilter(params); }
	String getTypeString() const override { return "Convert To Integer"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ToIntFilter)
};
