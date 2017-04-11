/*
  ==============================================================================

	ColorParameter.cpp
	Created: 11 Apr 2017 9:33:55am
	Author:  Ben

  ==============================================================================
*/

#include "ColorParameter.h"
#include "ColorParameterUI.h"

ColorParameter::ColorParameter(const String & niceName, const String & description, const Colour & initialColor, bool enabled) :
	Parameter(COLOR,niceName,description,var(),var(),var(),enabled)
{
	setColor(initialColor);
}

ColorParameter::~ColorParameter() {}

const Colour ColorParameter::getColor()
{
	if (value.size() < 4)
	{
		DBG("Error, color value size is " << value.size());
		return Colours::black;
	}

	return Colour((uint8)(int)value[0], (uint8)(int)value[1], (uint8)(int)value[2], (uint8)(int)value[3]);
}

void ColorParameter::setColor(const uint32 & _color, bool silentSet, bool force)
{
	setColor(Colour(_color),silentSet,force);
}

void ColorParameter::setColor(const Colour &_color, bool silentSet, bool force)
{
	var colorVar;
	colorVar.append(_color.getRed());
	colorVar.append(_color.getGreen());
	colorVar.append(_color.getBlue());
	colorVar.append(_color.getAlpha());
	setValue(colorVar, silentSet, force);
}

ColorParameterUI * ColorParameter::createColorParamUI()
{
	return new ColorParameterUI(this);
}

ControllableUI * ColorParameter::createDefaultUI(Controllable *)
{
	return createColorParamUI();
}
