/*
  ==============================================================================

    Point3DComparators.h
    Created: 2 Nov 2016 8:58:22pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../BaseComparator.h"

class Point3DComparator :
	public ParameterComparator
{
public:
	Point3DComparator(Array<WeakReference<Controllable>> sources);
	virtual ~Point3DComparator();

	const Identifier equalsId = "=";
	const Identifier distGreaterId = "d>";
	const Identifier distLessId = "d<";
	const Identifier magnGreaterId = "m>";
	const Identifier magnLessId = "m<";

	Array<Point3DParameter *> p3dParams;
	Point3DParameter * p3dRef;
	FloatParameter * valParam;

	virtual void compare(int iterationIndex) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Point3DComparator)
};