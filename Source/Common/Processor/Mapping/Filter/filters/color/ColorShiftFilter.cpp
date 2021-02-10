/*
  ==============================================================================

    ColorShiftFilter.cpp
    Created: 4 Jul 2018 2:20:55pm
    Author:  Ben

  ==============================================================================
*/

#include "ColorShiftFilter.h"

ColorShiftFilter::ColorShiftFilter(var params, Multiplex * multiplex) :
	MappingFilter(getTypeString(), params, multiplex)
{

	const String hsvNames[3] = { "Hue", "Saturation","Brightness" };
	for (int i = 0; i < 3; ++i)
	{
		hsvOffsets.add(filterParams.addFloatParameter(hsvNames[i], "The "+hsvNames[i]+" offset to shift to the source value", 0, -1, 1));
	}

	filterTypeFilters.add(Controllable::COLOR);
}

ColorShiftFilter::~ColorShiftFilter()
{
}

bool ColorShiftFilter::processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex)
{
	ColorParameter* tc = dynamic_cast<ColorParameter*>(out);

	jassert(tc != nullptr);
	if (tc == nullptr) return false;

	float hue, sat, bri;
	Colour c = ((ColorParameter*)source)->getColor();
	c.getHSB(hue, sat, bri);

	hue += (float)filterParams.getLinkedValue(hsvOffsets[0], multiplexIndex);
	sat += (float)filterParams.getLinkedValue(hsvOffsets[1], multiplexIndex);
	bri += (float)filterParams.getLinkedValue(hsvOffsets[2], multiplexIndex);

	tc->setColor(Colour::fromHSV(hue, sat, bri, c.getFloatAlpha()));

	return true;
}

