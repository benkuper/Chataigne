/*
  ==============================================================================

    Easing.h
    Created: 11 Dec 2016 1:29:02pm
    Author:  Ben

  ==============================================================================
*/

#ifndef EASING_H_INCLUDED
#define EASING_H_INCLUDED

#include "ControllableContainer.h"

class EasingUI;

class Easing :
	public ControllableContainer
{
public:
	enum Type { LINEAR, QUADRATIC, CUBIC, HOLD };

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

class QuadraticEasing :
	public Easing
{
public:
	QuadraticEasing();
	Point2DParameter * anchor;

	virtual float getValue(const float &start, const float &end, const float &weight) override
	{
		float relAnchor = jmap<float>(anchor->getPoint().y, start, end);
		
		float iw = 1 - weight;
		float x = iw * iw * 0 + 2 * iw * weight * anchor->getPoint().x + weight * weight * 1;

		float invX = 1 - x;
		float y = invX * invX * start + 2 * invX *x * relAnchor + x * x * end;
		
		return y;// map<float>(x, start, end);
	}
};


class CubicEasing :
	public Easing
{
public:
	CubicEasing();
	Point2DParameter * anchor1;
	Point2DParameter * anchor2;

	virtual float getValue(const float &start, const float &end, const float &weight) override
	{
		if (weight >= 1) return end;
		return start;
	}
};



#endif  // EASING_H_INCLUDED
