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
	
	filterTypeFilters.add(Controllable::FLOAT, Controllable::INT);
}

OffsetFilter::~OffsetFilter()
{
}

void OffsetFilter::processSingleParameterInternal(Parameter* source, Parameter* out)
{
	if (source->hasRange() && !source->value.isArray())
	{
		
		out->setNormalizedValue(source->getNormalizedValue());

	} else
	{
		out->setValue(source->floatValue() + offset->floatValue());
	}
}

void OffsetFilter::filterParamChanged(Parameter* fp)
{
	if (fp == offset)
	{
		for (int i=0;i<sourceParams.size();i++)
		{
			Parameter* p = filteredParameters[i];
			if (!filterTypeFilters.contains(p->type)) continue;

			float tMin = (float)sourceParams[i]->minimumValue + offset->floatValue();
			float tMax = (float)sourceParams[i]->maximumValue + offset->floatValue();
			if (tMin != (float)p->minimumValue || tMax != (float)p->maximumValue)
			{
				p->setRange(tMin, tMax);
			}
		}
		
	}
	
}
