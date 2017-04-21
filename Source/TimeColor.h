/*
  ==============================================================================

    TimeColor.h
    Created: 11 Apr 2017 11:40:21am
    Author:  Ben

  ==============================================================================
*/

#ifndef TIMECOLOR_H_INCLUDED
#define TIMECOLOR_H_INCLUDED

#include "ColorParameter.h"
#include "BaseItem.h"

class TimeColor :
	public BaseItem
{
public:
	TimeColor(float time = 0, const Colour & color = Colours::black, const String &name = "Color");
	virtual ~TimeColor();


	ColorParameter * color;
	FloatParameter * position;

	int gradientIndex;
};


#endif  // TIMECOLOR_H_INCLUDED
