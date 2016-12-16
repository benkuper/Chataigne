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
	enum Type { LINEAR, BEZIER, HOLD };

	Easing(Type type);
	virtual ~Easing();
	
	Type type;

	virtual float getValue(const float &start, const float &end, const float &weight) = 0;//must be overriden

	virtual EasingUI * createUI() = 0; //must be overriden
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Easing)

};


class LinearEasing :
	public Easing
{
public:
	LinearEasing();

	float getValue(const float &start, const float &end, const float &weight) override;

	EasingUI * createUI() override;
};

class HoldEasing :
	public Easing
{
public:
	HoldEasing();

	virtual float getValue(const float &start, const float &end, const float &weight) override;

	EasingUI * createUI() override;
};

class CubicEasing :
	public Easing
{
public:
	CubicEasing();
	Point2DParameter * anchor1;
	Point2DParameter * anchor2;

	virtual float getValue(const float &start, const float &end, const float &weight) override;

	Array<float> solveCubic(float a, float b, float c, float d);

	EasingUI * createUI() override;
};



#endif  // EASING_H_INCLUDED
