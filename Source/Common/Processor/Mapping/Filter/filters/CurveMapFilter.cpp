/*
  ==============================================================================

    CurveMapFilter.cpp
    Created: 4 Feb 2017 5:39:24pm
    Author:  Ben

  ==============================================================================
*/

#include "CurveMapFilter.h"

CurveMapFilter::CurveMapFilter(var params) :
	MappingFilter(getTypeString(), params),
	curve("Curve")
{
	curve.isSelectable = false;
	curve.length->setValue(1);
	curve.addItem(0, 0, false);
	curve.items[0]->setEasing(Easing::BEZIER);
	curve.addItem(1, 1, false);
	curve.enableSnap->setValue(false);
	curve.showUIInEditor = true;
	curve.selectItemWhenCreated = false;
	filterParams.addChildControllableContainer(&curve);

	forceOutParameterType = FloatParameter::getTypeStringStatic();
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

