/*
  ==============================================================================

    TimeColor.h
    Created: 11 Apr 2017 11:40:21am
    Author:  Ben

  ==============================================================================
*/

#ifndef TIMECOLOR_H_INCLUDED
#define TIMECOLOR_H_INCLUDED

#include "JuceHeader.h"

class TimeColor :
	public BaseItem
{
public:
	TimeColor(float time = 0, const Colour & color = Colours::black, const String &name = "Color");
	virtual ~TimeColor();

	enum Interpolation { LINEAR, NONE };
	ColorParameter * color;
	FloatParameter * position;
	EnumParameter * interpolation;

	int gradientIndex;
};


#endif  // TIMECOLOR_H_INCLUDED
