/*
  ==============================================================================

    Point3DComparators.cpp
    Created: 2 Nov 2016 8:58:22pm
    Author:  bkupe

  ==============================================================================
*/

#include "Point3DComparators.h"

Point3DComparator::Point3DComparator(Parameter * sourceParam)
{
	p3dRef = addPoint3DParameter("Reference", "Comparison Reference to check against source value", sourceParam->value);
	p3dRef->setRange(sourceParam->minimumValue, sourceParam->maximumValue);
	p3dRef->setValue(sourceParam->value, false, true, true);
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

bool Point3DComparator::compare(Parameter *sourceParam)
{
    
	Vector3D<float> p = ((Point3DParameter *)sourceParam)->getVector();
	Vector3D<float> r = p3dRef->getVector();
	
	if (currentFunctionId == equalsId)				return p.x == r.x && p.y == r.y && p.z == r.z;
	else if (currentFunctionId == distGreaterId)	return (p - r).length() > valParam->floatValue();
	else if (currentFunctionId == distLessId)		return (p - r).length() < valParam->floatValue();
	else if (currentFunctionId == magnGreaterId)	return p.length() > valParam->floatValue();
	else if (currentFunctionId == magnLessId)		return p.length() < valParam->floatValue();	
}
