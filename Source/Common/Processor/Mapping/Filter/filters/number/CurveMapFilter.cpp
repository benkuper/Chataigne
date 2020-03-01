/*
  ==============================================================================

    CurveMapFilter.cpp
    Created: 4 Feb 2017 5:39:24pm
    Author:  Ben

  ==============================================================================
*/

#include "CurveMapFilter.h"

/*
CurveMapFilter::CurveMapFilter(var params) :
	MappingFilter(getTypeString(), params),
	curve("Curve")
{
	curve.isSelectable = false;
	curve.length->setValue(1);
	curve.addItem(0, 0, false);
	curve.items[0]->easingType->setValueWithData(Easing::BEZIER);
	curve.addItem(1, 1, false);
	curve.enableSnap->setValue(false);
	curve.showUIInEditor = true;
	curve.selectItemWhenCreated = false;
	curve.hideEditorHeader = true;
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

void CurveMapFilter::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c)
{
	if (c == curve.value || c == curve.position) return; //avoid value change to be notifying the mapping, it would be recognized as a filter parameter and would trigger a new process
	MappingFilter::onControllableFeedbackUpdateInternal(cc, c);
}

var CurveMapFilter::getJSONData()
{
	var data = MappingFilter::getJSONData();
	data.getDynamicObject()->setProperty(curve.shortName, curve.getJSONData());
	return data;
}

void CurveMapFilter::loadJSONDataInternal(var data)
{
	MappingFilter::loadJSONDataInternal(data);
	curve.loadJSONData(data.getProperty(curve.shortName, var()), true);
}

*/