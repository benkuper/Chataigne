/*
  ==============================================================================

    Point2DComparators.cpp
    Created: 2 Nov 2016 8:58:14pm
    Author:  bkupe

  ==============================================================================
*/

#include "Point2DComparators.h"


Point2DComparator::Point2DComparator(Array<WeakReference<Controllable>> sources) :
	ParameterComparator(sources)
{
	p2dParams.addArray(sources);

	p2dRef = addPoint2DParameter("Reference", "Comparison Reference to check against source value");
	p2dRef->setRange(p2dParams[0]->minimumValue, p2dParams[0]->maximumValue);
	p2dRef->setValue(p2dParams[0]->value, false, true, true);

	reference = p2dRef;

	valParam = addFloatParameter("Reference 2", "Depending on the comparison function, will act as reference for distance, magniture or other value reference to check against.", 0);
	
	addCompareOption("=", equalsId);
	addCompareOption("Distance >", distGreaterId);
	addCompareOption("Distance <", distLessId);
	addCompareOption("Magnitude >", magnGreaterId);
	addCompareOption("Magnitude <", magnLessId);


	}

Point2DComparator::~Point2DComparator()
{
}

void Point2DComparator::compare(int iterationIndex)
{
	Point<float> p = p2dParams[iterationIndex]->getPoint();
	Point<float> r = p2dRef->getPoint();
	if (currentFunctionId == equalsId)				setValid(iterationIndex, p == r);
	else if (currentFunctionId == distGreaterId)	setValid(iterationIndex, p.getDistanceFrom(r) > valParam->floatValue());
	else if (currentFunctionId == distLessId)		setValid(iterationIndex, p.getDistanceFrom(r) > valParam->floatValue());
	else if (currentFunctionId == magnGreaterId)	setValid(iterationIndex, p.getDistanceFromOrigin() >valParam->floatValue());
	else if (currentFunctionId == magnLessId)		setValid(iterationIndex, p.getDistanceFromOrigin() < valParam->floatValue());
}