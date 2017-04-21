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
	Parameter(COLOR,niceName,description,var(),var(),var(),enabled),
	mode(FLOAT)
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

	if (mode == FLOAT) return Colour((uint8)((float)value[0]*255), (uint8)((float)value[1]*255), (uint8)((float)value[2]*255), (uint8)((float)value[3]*255));
	else return Colour((uint8)(int)value[0], (uint8)(int)value[1], (uint8)(int)value[2], (uint8)(int)value[3]);
}

void ColorParameter::setFloatRGBA(const float & r, const float & g, const float & b, const float & a)
{
	setColor(Colour::fromFloatRGBA(r, g, b, a));
}

void ColorParameter::setColor(const uint32 & _color, bool silentSet, bool force)
{
	setColor(Colour(_color),silentSet,force);
}

void ColorParameter::setColor(const Colour &_color, bool silentSet, bool force)
{
	var colorVar;
	if (mode == FLOAT)
	{
		colorVar.append(_color.getFloatRed());
		colorVar.append(_color.getFloatGreen());
		colorVar.append(_color.getFloatBlue());
		colorVar.append(_color.getFloatAlpha());
	}
	else
	{
		colorVar.append(_color.getRed());
		colorVar.append(_color.getGreen());
		colorVar.append(_color.getBlue());
		colorVar.append(_color.getAlpha());
	}
	
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
