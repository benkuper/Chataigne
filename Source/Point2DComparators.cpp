/*
  ==============================================================================

    Point2DComparators.cpp
    Created: 2 Nov 2016 8:58:14pm
    Author:  bkupe

  ==============================================================================
*/

#include "Point2DComparators.h"


Point2DComparator::Point2DComparator(Controllable * c) :
	ParameterComparator(c),
	p2dParam((Point2DParameter *)c)
{
	addCompareOption("=", equalsId);
	addCompareOption("Distance >", distGreaterId);
	addCompareOption("Distance <", distLessId);
	addCompareOption("Magnitude >", magnGreaterId);
	addCompareOption("Magnitude <", magnLessId);


	ref = addPoint2DParameter("Reference", "Comparison Reference to check against source value", p2dParam->defaultValue);
	ref->setBounds(p2dParam->minX, p2dParam->minY, p2dParam->maxX, p2dParam->maxY);

	valParam = addFloatParameter("Reference 2", "Depending on the comparison function, will act as reference for distance, magniture or other value reference to check against.", 0);
}

Point2DComparator::~Point2DComparator()
{
}

void Point2DComparator::compare()
{
	Point<float> p = p2dParam->getPoint();
	Point<float> r = ref->getPoint();
	if (currentFunctionId == equalsId)				setValid(p == r);
	else if (currentFunctionId == distGreaterId)	setValid(p.getDistanceFrom(r) > valParam->floatValue());
	else if (currentFunctionId == distLessId)		setValid(p.getDistanceFrom(r) > valParam->floatValue());
	else if (currentFunctionId == magnGreaterId)	setValid(p.getDistanceFromOrigin() >valParam->floatValue());
	else if (currentFunctionId == magnLessId)		setValid(p.getDistanceFromOrigin() < valParam->floatValue());
}