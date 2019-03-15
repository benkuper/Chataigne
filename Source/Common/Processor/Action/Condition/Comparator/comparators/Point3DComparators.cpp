/*
  ==============================================================================

    Point3DComparators.cpp
    Created: 2 Nov 2016 8:58:22pm
    Author:  bkupe

  ==============================================================================
*/

#include "Point3DComparators.h"

Point3DComparator::Point3DComparator(Controllable * c) :
	ParameterComparator(c),
	p3dParam((Point3DParameter *)c)
{
	p3dRef = addPoint3DParameter("Reference", "Comparison Reference to check against source value", p3dParam->value);
	p3dRef->setRange(p3dParam->minimumValue, p3dParam->maximumValue, true);
	p3dRef->setValue(p3dParam->value, false, true, true);
	reference = p3dRef;

	valParam = addFloatParameter("Reference 2", "Depending on the comparison function, will act as reference for distance, magniture or other value reference to check against.", 0);
	
	addCompareOption("=", equalsId);
	addCompareOption("Distance >", distGreaterId);
	addCompareOption("Distance <", distLessId);
	addCompareOption("Magnitude >", magnGreaterId);
	addCompareOption("Magnitude <", magnLessId);

}

Point3DComparator::~Point3DComparator()
{
}

void Point3DComparator::compare()
{
    /*
	Vector3D<float> p = p3dParam->getVector();
	Vector3D<float> r = p3dRef->getVector();
	
	if (currentFunctionId == equalsId)				setValid(p.);
	else if (currentFunctionId == distGreaterId)	setValid(p.getDistanceFrom(r) > valParam->floatValue());
	else if (currentFunctionId == distLessId)		setValid(p.getDistanceFrom(r) > valParam->floatValue());
	else if (currentFunctionId == magnGreaterId)	setValid(p.getDistanceFromOrigin() >valParam->floatValue());
	else if (currentFunctionId == magnLessId)		setValid(p.getDistanceFromOrigin() < valParam->floatValue());
     */
	
}
