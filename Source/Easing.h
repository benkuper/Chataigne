/*
  ==============================================================================

    Easing.h
    Created: 11 Dec 2016 1:29:02pm
    Author:  Ben

  ==============================================================================
*/

#ifndef EASING_H_INCLUDED
#define EASING_H_INCLUDED

#include "JuceHeader.h"

class Easing
{
public:
	enum Type { LINEAR, EASE_IN, EASE_OUT, HOLD };

	Easing(Type type);
	virtual ~Easing();
	
	Type type;
	virtual float getValue(const float &start, const float &end, const float &weight) = 0;//must be overriden
};


class LinearEasing :
	public Easing
{
public:
	LinearEasing() : Easing(LINEAR) {}

	virtual float getValue(const float &start, const float &end, const float &weight) override
	{
		return start + (end - start)*weight;
	}
};

class HoldEasing :
	public Easing
{
public:
	HoldEasing() : Easing(HOLD) {}

	virtual float getValue(const float &start, const float &end, const float &weight) override
	{
		if (weight >= 1) return end;
		return start;
	}
};




#endif  // EASING_H_INCLUDED
