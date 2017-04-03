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
	enum Type { LINEAR, BEZIER, HOLD, SINE };

	Easing(Type type);
	virtual ~Easing();
	
	Type type;

	virtual float getValue(const float &start, const float &end, const float &weight) = 0;//must be overriden

	virtual EasingUI * createUI() = 0; //must be overriden
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Easing)

private :
	WeakReference<Easing>::Master masterReference;
	friend class WeakReference<Easing>;

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

	void onContainerParameterChanged(Parameter * p) override;
	class Bezier
	{
	public:
		Point<float> a;
		Point<float> b;
		Point<float> c;

		const float epsilon = 1e-6f; //Precision

		void setup(const Point<float> &a1, const Point<float> &a2);

		inline float sampleCurveX(float t);
		inline float sampleCurveY(float t);
		inline float sampleCurveDerivativeX(float t);

		float getValueForX(const float &tx);

		float solveCurveX(const float &tx);
	};

	Bezier bezier;
	//Array<float> solveCubic(float a, float b, float c, float d);

	EasingUI * createUI() override;
};


class SineEasing :
	public Easing
{
public:
	SineEasing();
	Point2DParameter * freqAmp;

	virtual float getValue(const float &start, const float &end, const float &weight) override;

	EasingUI * createUI() override;
};

#endif  // EASING_H_INCLUDED
