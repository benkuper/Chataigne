/*
  ==============================================================================

    Easing.cpp
    Created: 11 Dec 2016 1:29:02pm
    Author:  Ben

  ==============================================================================
*/

#include "Easing.h"
#include "EasingUI.h"

Easing::Easing(Type _type) :
ControllableContainer("ease"),
type(_type)
{
	showInspectorOnSelect = false;
}

Easing::~Easing()
{
	masterReference.clear();
}


LinearEasing::LinearEasing() : Easing(LINEAR) {}
HoldEasing::HoldEasing() : Easing(HOLD) {}


EasingUI * LinearEasing::createUI()
{
	return new LinearEasingUI(this);
}


EasingUI * HoldEasing::createUI()
{
	return new HoldEasingUI(this);
}

EasingUI * CubicEasing::createUI()
{
	return new CubicEasingUI(this);
}


float LinearEasing::getValue(const float & start, const float & end, const float & weight)
{
	return start + (end - start)*weight;
}



float HoldEasing::getValue(const float & start, const float & end, const float & weight)
{
	if (weight >= 1) return end;
	return start;
}


CubicEasing::CubicEasing() :
	Easing(BEZIER)
{
	anchor1 = addPoint2DParameter("Anchor 1", "Anchor 1 of the quadratic curve");
	anchor1->setBounds(0, -1, .99f, 2);

	anchor2 = addPoint2DParameter("Anchor 2", "Anchor 2 of the quadratic curve");
	anchor2->setBounds(.01f, -1, 1, 2);
	
	anchor1->setPoint(.6f, 0);
	anchor2->setPoint(.7f, 1);

	bezier.setup(anchor1->getPoint(), anchor2->getPoint());
}


void CubicEasing::onContainerParameterChanged(Parameter * p)
{
	if(p == anchor1 || p == anchor2) bezier.setup(anchor1->getPoint(), anchor2->getPoint());
}


float CubicEasing::getValue(const float & start, const float & end, const float & weight)
{
	if (weight <= 0) return start;
	if (weight >= 1) return end;

	float val =  bezier.getValueForX(weight);
	return jmap<float>(val, start, end);
}

void CubicEasing::Bezier::setup(const Point<float>& a1, const Point<float>& a2)
{
	c.setXY(3 * a1.x, 3 * a1.y);
	b.setXY(3 * (a2.x - a1.x) - c.x, 3 * (a2.y - a1.y) - c.y);
	a.setXY(1 - c.x - b.x, 1 - c.y - b.y);
}

inline float CubicEasing::Bezier::sampleCurveX(float t) {
	return ((a.x * t + b.x) * t + c.x) * t;
}

inline float CubicEasing::Bezier::sampleCurveY(float t) {
	return ((a.y * t + b.y) * t + c.y) * t;
}

inline float CubicEasing::Bezier::sampleCurveDerivativeX(float t) {
	return (3 * a.x * t + 2 * b.x) * t + c.x;
}

float CubicEasing::Bezier::getValueForX(const float & tx)
{
	return sampleCurveY(solveCurveX(tx));
}

float CubicEasing::Bezier::solveCurveX(const float & tx)
{
	float t0;
	float t1;
	float t2;
	float x2;
	float d2;
	float i;

	// First try a few iterations of Newton's method -- normally very fast.
	for (t2 = tx, i = 0; i < 8; i++) {
		x2 = sampleCurveX(t2) - tx;
		if (std::abs(x2) < epsilon)
			return t2;
		d2 = sampleCurveDerivativeX(t2);
		if (std::abs(d2) < epsilon)
			break;
		t2 = t2 - x2 / d2;
	}

	// No solution found - use bi-section
	t0 = 0.0;
	t1 = 1.0;
	t2 = tx;

	if (t2 < t0) return t0;
	if (t2 > t1) return t1;

	while (t0 < t1) {
		x2 = sampleCurveX(t2);
        if (std::abs(x2 - tx) < epsilon)
			return t2;
		if (tx > x2) t0 = t2;
		else t1 = t2;

		t2 = (t1 - t0) * .5f + t0;
	}

	// Give up
	return t2;
}

SineEasing::SineEasing() :
	Easing(SINE)
{
	freqAmp = addPoint2DParameter("Anchor 1", "Anchor 1 of the quadratic curve");
	freqAmp->setBounds(.01f, -1, 1, 2);
	freqAmp->setPoint(.2f, .5f);
}

float SineEasing::getValue(const float & start, const float & end, const float & weight)
{
	//DBG(freqAmp->getPoint().toString() << " / " << sinf(weight / freqAmp->x));
	return start + (end - start)*weight + sinf(weight*float_Pi*2/freqAmp->x)*freqAmp->y;
}


EasingUI * SineEasing::createUI()
{
	return new SineEasingUI(this);
}
