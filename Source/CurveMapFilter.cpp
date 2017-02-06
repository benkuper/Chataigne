/*
  ==============================================================================

    CurveMapFilter.cpp
    Created: 4 Feb 2017 5:39:24pm
    Author:  Ben

  ==============================================================================
*/

#include "CurveMapFilter.h"
#include "CurveMapFilterEditor.h"

CurveMapFilter::CurveMapFilter() :
	MappingFilter(getTypeString())
{
	curve.setPositionMax(1);
	curve.addItem(0, 0);
	curve.addItem(1, 1);
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


/*
InspectableEditor * CurveMapFilter::getEditor(bool isRoot)
{
	return new CurveMapFilterEditor(this, isRoot);
}
*/
