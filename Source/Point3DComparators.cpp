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
	addCompareOption("=", equalsId);
	addCompareOption("Distance >", distGreaterId);
	addCompareOption("Distance <", distLessId);
	addCompareOption("Magnitude >", magnGreaterId);
	addCompareOption("Magnitude <", magnLessId);


	ref = addPoint3DParameter("Reference", "Comparison Reference to check against source value", p3dParam->defaultValue);
	ref->setBounds(p3dParam->minX, p3dParam->minY, p3dParam->minZ, p3dParam->maxX,p3dParam->maxY,p3dParam->maxZ);

	valParam = addFloatParameter("Reference 2", "Depending on the comparison function, will act as reference for distance, magniture or other value reference to check against.", 0);
}

Point3DComparator::~Point3DComparator()
{
}

void Point3DComparator::compare()
{
	Vector3D<float> p = p3dParam->getVector();
	Vector3D<float> r = ref->getVector();
	/*
	if (currentFunctionId == equalsId)				setValid(p.);
	else if (currentFunctionId == distGreaterId)	setValid(p.getDistanceFrom(r) > valParam->floatValue());
	else if (currentFunctionId == distLessId)		setValid(p.getDistanceFrom(r) > valParam->floatValue());
	else if (currentFunctionId == magnGreaterId)	setValid(p.getDistanceFromOrigin() >valParam->floatValue());
	else if (currentFunctionId == magnLessId)		setValid(p.getDistanceFromOrigin() < valParam->floatValue());
	*/
}