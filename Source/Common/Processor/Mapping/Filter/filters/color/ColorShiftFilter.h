/*
  ==============================================================================

    ColorShiftFilter.h
    Created: 4 Jul 2018 2:20:55pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../../MappingFilter.h"

class ColorShiftFilter :
	public MappingFilter
{
public:
	ColorShiftFilter(var params, Multiplex* multiplex);
	~ColorShiftFilter();

	Array<FloatParameter*> hsvOffsets;

	bool processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex) override;

	virtual String getTypeString() const override { return getTypeStringStatic(); }
	static const String getTypeStringStatic() { return "HSV Adjust"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ColorShiftFilter)
};
