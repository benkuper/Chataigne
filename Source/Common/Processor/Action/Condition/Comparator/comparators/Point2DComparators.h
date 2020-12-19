/*
  ==============================================================================

    Point2DComparators.h
    Created: 2 Nov 2016 8:58:14pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../BaseComparator.h"

class Point2DComparator :
	public ParameterComparator
{
public:
	Point2DComparator(Array<WeakReference<Controllable>> sources);
	virtual ~Point2DComparator();

	const Identifier equalsId = "=";
	const Identifier distGreaterId = "d>";
	const Identifier distLessId = "d<";
	const Identifier magnGreaterId = "m>";
	const Identifier magnLessId = "m<";


	Array<Point2DParameter *> p2dParams;
	Point2DParameter * p2dRef;
	FloatParameter * valParam;

	virtual void compare(int iterationIndex) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Point2DComparator)
};