/*
  ==============================================================================

    CropFilter.cpp
    Created: 26 Jan 2018 11:39:14am
    Author:  Ben

  ==============================================================================
*/

#include "CropFilter.h"

CropFilter::CropFilter(var params) :
	MappingFilter(getTypeString(), params)
{

	targetMin = filterParams.addFloatParameter("Target Min", "New minimum for output", 0, 0, 1);
	targetMax = filterParams.addFloatParameter("Target Max", "New maximum for output", 1, 0, 1);
	targetMin->defaultUI = FloatParameter::LABEL;
	targetMax->defaultUI = FloatParameter::LABEL;
	targetMin->isCustomizableByUser = false;
	targetMax->isCustomizableByUser = false;

	autoSetRange = false;
}

CropFilter::~CropFilter()
{
}

void CropFilter::processInternal()
{
	filteredParameter->setValue(sourceParam->value);
}


void CropFilter::filterParamChanged(Parameter * p)
{
	if (p == targetMin || p == targetMax)
	{
		if (filteredParameter != nullptr) filteredParameter->setRange(targetMin->floatValue(), jmax<float>(targetMax->floatValue(), targetMin->floatValue()));
	}
}

Parameter * CropFilter::setupParameterInternal(Parameter * source)
{
	Parameter * p = MappingFilter::setupParameterInternal(source);
	if (p->isComplex())
	{
		var minVal;
		var maxVal;
		for (int i = 0; i < p->value.size(); i++)
		{
			minVal.append(targetMin->floatValue());
			maxVal.append(jmax<float>(targetMax->floatValue(), targetMin->floatValue()));
		}

		p->setRange(minVal, maxVal);
	}
	else
	{
		p->setRange(targetMin->floatValue(), jmax<float>(targetMax->floatValue(), targetMin->floatValue()));
	}
	return p;
}
