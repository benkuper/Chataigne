/*
  ==============================================================================

    CurveMapFilter.cpp
    Created: 4 Feb 2017 5:39:24pm
    Author:  Ben

  ==============================================================================
*/

#include "CurveMapFilter.h"

CurveMapFilter::CurveMapFilter(var params) :
	MappingFilter(getTypeString(), params)
{
	curve.setPositionMax(1);
	curve.addItem(0, 0);
	curve.addItem(1, 1);
	curve.enableSnap->setValue(false);
	curve.showUIInEditor = true;
	curve.selectItemWhenCreated = false;
	filterParams.addChildControllableContainer(&curve);
}

CurveMapFilter::~CurveMapFilter()
{
}

void CurveMapFilter::processInternal()
{
	curve.position->setValue(sourceParam->getNormalizedValue());
	filteredParameter->setNormalizedValue(curve.value->floatValue());
}

var CurveMapFilter::getJSONData()
{
	var data = MappingFilter::getJSONData();
	data.getDynamicObject()->setProperty("curve", curve.getJSONData());
	return data;
}

void CurveMapFilter::loadJSONDataInternal(var data)
{
	MappingFilter::loadJSONDataInternal(data);
	curve.loadJSONData(data.getProperty("curve", var()), true);
}

Parameter * CurveMapFilter::setupParameterInternal(Parameter * source, const String & forceType)
{
	return MappingFilter::setupParameterInternal(source, FloatParameter::getTypeStringStatic());
}


/*
InspectableEditor * CurveMapFilter::getEditor(bool isRoot)
{
	return new CurveMapFilterEditor(this, isRoot);
}
*/
