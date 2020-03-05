/*
  ==============================================================================

    ColorShiftFilter.cpp
    Created: 4 Jul 2018 2:20:55pm
    Author:  Ben

  ==============================================================================
*/

#include "ColorShiftFilter.h"

ColorShiftFilter::ColorShiftFilter(var params) :
	MappingFilter(getTypeString(), params)
{

	const String hsvNames[3] = { "Hue", "Saturation","Brightness" };
	for (int i = 0; i < 3; i++)
	{
		hsvOffsets.add(filterParams.addFloatParameter(hsvNames[i], "The "+hsvNames[i]+" offset to shift to the source value", 0, -1, 1));
	}

	filterTypeFilters.add(Controllable::COLOR);
}

ColorShiftFilter::~ColorShiftFilter()
{
}

void ColorShiftFilter::processSingleParameterInternal(Parameter * source, Parameter * out)
{
	ColorParameter* tc = (ColorParameter*)out;

	float hue, sat, bri;
	Colour c = ((ColorParameter*)source)->getColor();
	c.getHSB(hue, sat, bri);
	hue += hsvOffsets[0]->floatValue();
	sat += hsvOffsets[1]->floatValue();
	bri += hsvOffsets[2]->floatValue();
	tc->setColor(Colour::fromHSV(hue, sat, bri, c.getFloatAlpha()));
}

