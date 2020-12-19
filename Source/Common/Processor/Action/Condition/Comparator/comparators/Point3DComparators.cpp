/*
  ==============================================================================

    Point3DComparators.cpp
    Created: 2 Nov 2016 8:58:22pm
    Author:  bkupe

  ==============================================================================
*/

#include "Point3DComparators.h"

Point3DComparator::Point3DComparator(Array<WeakReference<Controllable>> sources) :
	ParameterComparator(sources)
{
	p3dParams.addArray(sources);

	p3dRef = addPoint3DParameter("Reference", "Comparison Reference to check against source value", p3dParams[0]->value);
	p3dRef->setRange(p3dParams[0]->minimumValue, p3dParams[0]->maximumValue);
	p3dRef->setValue(p3dParams[0]->value, false, true, true);
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

void Point3DComparator::compare(int iterationIndex)
{
    
	Vector3D<float> p = p3dParams[0]->getVector();
	Vector3D<float> r = p3dRef->getVector();
	
	if (currentFunctionId == equalsId)				setValid(iterationIndex, p.x == r.x && p.y == r.y && p.z == r.z);
	else if (currentFunctionId == distGreaterId)	setValid(iterationIndex, (p - r).length() > valParam->floatValue());
	else if (currentFunctionId == distLessId)		setValid(iterationIndex, (p - r).length() < valParam->floatValue());
	else if (currentFunctionId == magnGreaterId)	setValid(iterationIndex, p.length() > valParam->floatValue());
	else if (currentFunctionId == magnLessId)		setValid(iterationIndex, p.length() < valParam->floatValue());	
}
