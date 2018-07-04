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
	ColorShiftFilter(var params);
	~ColorShiftFilter();

	enum ShiftMode { HUE, BRIGHTNESS, SATURATION };
	
	EnumParameter * mode;
	FloatParameter * offset;

	void processInternal() override;

	static ColorShiftFilter * create(var params) { return new ColorShiftFilter(params); }
	virtual String getTypeString() const override { return "ColorShift"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ColorShiftFilter)
};

