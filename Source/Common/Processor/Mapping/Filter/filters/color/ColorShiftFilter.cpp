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

	forceOutParameterType = ColorParameter::getTypeStringStatic();
}

ColorShiftFilter::~ColorShiftFilter()
{
}

void ColorShiftFilter::processInternal()
{
	ColorParameter * tc = (ColorParameter *)filteredParameter.get();

	if (tc == nullptr)
	{
		DBG("ERROR HERE, should not be null");
		return;
	}
	
	if (sourceParam == nullptr)
	{
		DBG("ERROR HERE, source should not be null");
		return;
	}

	Colour c = Colours::black;
	if (sourceParam->type != Parameter::COLOR)
	{
		float h = 0;
		float b = 1;
		float s = 1;

		if (sourceParam->isComplex())
		{
			h = sourceParam->value[0];
			b = sourceParam->value[1];
			s = sourceParam->value.size() >= 3?(float)sourceParam->value[2]:1;
		} else
		{
			h = sourceParam->floatValue();
		}

		c = Colour::fromHSV(h, s, b, 1);
	} else
	{
		c = ((ColorParameter *)sourceParam.get())->getColor();
	}

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
