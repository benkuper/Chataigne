/*
  ==============================================================================

    InverseFilter.cpp
    Created: 4 Feb 2017 5:39:15pm
    Author:  Ben

  ==============================================================================
*/

#include "InverseFilter.h"

InverseFilter::InverseFilter() :
	MappingFilter(getTypeString())
{
}

InverseFilter::~InverseFilter()
{
}

void InverseFilter::processInternal(Parameter * source, Parameter * target)
{
	target->setValue(jmap<float>(source->getNormalizedValue(), source->maximumValue, source->minimumValue));
}
