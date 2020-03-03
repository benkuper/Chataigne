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
	mode = filterParams.addEnumParameter("Mode", "The shift mode of the filter");
	mode->addOption("Hue", HUE)->addOption("Saturation", SATURATION)->addOption("Brightness", BRIGHTNESS);

	offset = filterParams.addFloatParameter("Offset", "The offset to shift to the source value", .5f, -1, 1);

	filterTypeFilters.add(Controllable::COLOR);
}

ColorShiftFilter::~ColorShiftFilter()
{
}

void ColorShiftFilter::processSingleParameterInternal(Parameter * source, Parameter * out)
{
	ColorParameter* tc = (ColorParameter*)out;

	if (tc == nullptr)
	{
		DBG("ERROR HERE, should not be null");
		return;
	}
	
	if (source == nullptr)
	{
		DBG("ERROR HERE, source should not be null");
		return;
	}

	Colour c = ((ColorParameter *)source)->getColor();

	ShiftMode m = mode->getValueDataAsEnum<ShiftMode>();
	switch (m)
	{
	case HUE:
		tc->setColor(c.withHue(c.getHue() + offset->floatValue()));
		break;

	case SATURATION:
		tc->setColor(c.withSaturation(c.getSaturation() + offset->floatValue()));
		break;

	case BRIGHTNESS:
		tc->setColor(c.withBrightness(c.getBrightness() + offset->floatValue()));
		break;
	}
}

