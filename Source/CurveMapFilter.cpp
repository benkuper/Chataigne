/*
  ==============================================================================

    CurveMapFilter.cpp
    Created: 4 Feb 2017 5:39:24pm
    Author:  Ben

  ==============================================================================
*/

#include "CurveMapFilter.h"

CurveMapFilter::CurveMapFilter() :
	MappingFilter(getTypeString())
{
	curve.setPositionMax(1);
	curve.showUIInEditor = true;
	curve.selectItemWhenCreated = false;
	filterParams.addChildControllableContainer(&curve);
}

CurveMapFilter::~CurveMapFilter()
{
}

void CurveMapFilter::processInternal()
{
	filteredParameter->setNormalizedValue(curve.getValueForPosition(sourceParam->getNormalizedValue()));
}