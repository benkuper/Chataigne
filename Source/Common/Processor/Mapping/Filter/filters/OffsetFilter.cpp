/*
  ==============================================================================

    OffsetFilter.cpp
    Created: 19 Jan 2018 11:20:10am
    Author:  Ben

  ==============================================================================
*/

#include "OffsetFilter.h"

OffsetFilter::OffsetFilter(var params) :
	MappingFilter(getTypeString(),params)
{
	autoSetRange = false;
	offset = filterParams.addFloatParameter("Offset", "The amount of offset to apply", 0);
	offset->isCustomizableByUser = true;
	forceOutParameterType = FloatParameter::getTypeStringStatic();
}

OffsetFilter::~OffsetFilter()
{
}

void OffsetFilter::processInternal()
{
	if (sourceParam->hasRange() && !sourceParam->value.isArray())
	{
		float tMin = (float)sourceParam->minimumValue + offset->floatValue();
		float tMax = (float)sourceParam->maximumValue + offset->floatValue();
		if (tMin != (float)filteredParameter->minimumValue || tMax != (float)filteredParameter->maximumValue)
		{
			filteredParameter->setRange(tMin, tMax);
		}
		filteredParameter->setNormalizedValue(sourceParam->getNormalizedValue());

	} else
	{
		filteredParameter->setValue(sourceParam->floatValue() + offset->floatValue());
	}
}