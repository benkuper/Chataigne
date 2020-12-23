/*
  ==============================================================================

    Point2DComparators.cpp
    Created: 2 Nov 2016 8:58:14pm
    Author:  bkupe

  ==============================================================================
*/

#include "Point2DComparators.h"


Point2DComparator::Point2DComparator(Parameter * sourceParam)
{
	p2dRef = addPoint2DParameter("Reference", "Comparison Reference to check against source value");
	p2dRef->setRange(sourceParam->minimumValue, sourceParam->maximumValue);
	p2dRef->setValue(sourceParam->value, false, true, true);
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

bool Point2DComparator::compare(Parameter* sourceParam)
{
	Point<float> p = ((Point2DParameter *)sourceParam)->getPoint();
	Point<float> r = p2dRef->getPoint();
	if (currentFunctionId == equalsId)				return p == r;
	else if (currentFunctionId == distGreaterId)	return p.getDistanceFrom(r) > valParam->floatValue();
	else if (currentFunctionId == distLessId)		return p.getDistanceFrom(r) > valParam->floatValue();
	else if (currentFunctionId == magnGreaterId)	return p.getDistanceFromOrigin() >valParam->floatValue();
	else if (currentFunctionId == magnLessId)		return p.getDistanceFromOrigin() < valParam->floatValue();
	return false;
}