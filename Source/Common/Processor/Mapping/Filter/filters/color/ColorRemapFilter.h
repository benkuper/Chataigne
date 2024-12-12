/*
  ==============================================================================

    ColorRemapFilter.h
    Created: 29 Mar 2021 6:22:21pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ColorRemapFilter :
	public MappingFilter
{
public:
	ColorRemapFilter(var params, Multiplex* multiplex);
	~ColorRemapFilter();

	BoolParameter* useCustomInputRange;
	Point2DParameter* targetIn;
	GradientColorManager gradientManager;

	Parameter* setupSingleParameterInternal(Parameter* source, int multiplexIndex, bool rangeOnly) override;
	ProcessResult processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex) override;

	var getRemappedValueFor(Parameter* source, int multiplexIndex); //allow for child classes to invoke this 

	void filterParamChanged(Parameter*) override;

	var getJSONData(bool includeNonOverriden = false) override;
	void loadJSONDataItemInternal(var data) override;

	String getTypeString() const override { return getTypeStringStatic(); }
	static String getTypeStringStatic() { return "Color Remap"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ColorRemapFilter)
};