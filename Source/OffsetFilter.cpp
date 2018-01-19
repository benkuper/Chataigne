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
	offset = filterParams.addFloatParameter("Offset", "The amount of offset to apply", .5f, -1, 1);
	offset->isCustomizableByUser = true;
	forceOutParameterType = FloatParameter::getTypeStringStatic();
}

OffsetFilter::~OffsetFilter()
{
}

void OffsetFilter::processInternal()
{
	filteredParameter->setValue(sourceParam->floatValue() + offset->floatValue());
}

Parameter * OffsetFilter::setupParameterInternal(Parameter * source)
{ 
	Parameter * p = MappingFilter::setupParameterInternal(source);
	offset->setRange((float)p->minimumValue-((float)p->maximumValue-(float)p->minimumValue), p->maximumValue);
	return p;
}
