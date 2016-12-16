/*
  ==============================================================================

    Easing.cpp
    Created: 11 Dec 2016 1:29:02pm
    Author:  Ben

  ==============================================================================
*/

#include "Easing.h"

Easing::Easing(Type _type) :
	type(_type),
	ControllableContainer("ease")
{
}

Easing::~Easing()
{
}

QuadraticEasing::QuadraticEasing() :
	Easing(QUADRATIC)
{
	anchor = addPoint2DParameter("Anchor", "Anchor of the quadratic curve");
	anchor->setPoint(.8f, .2f);
}

CubicEasing::CubicEasing() :
	Easing(CUBIC)
{
	anchor1 = addPoint2DParameter("Anchor 1", "Anchor 1 of the quadratic curve");
	anchor1->setPoint(.5f, .5f);

	anchor2 = addPoint2DParameter("Anchor 2", "Anchor 2 of the quadratic curve");
	anchor2->setPoint(.5f, .5f);
}
