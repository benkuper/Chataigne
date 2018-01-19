/*
  ==============================================================================

    InverseFilter.cpp
    Created: 4 Feb 2017 5:39:15pm
    Author:  Ben

  ==============================================================================
*/

#include "InverseFilter.h"

InverseFilter::InverseFilter(var params) :
	MappingFilter(getTypeString(), params)
{
	editorCanBeCollapsed = false;
	editorIsCollapsed = true;
}

InverseFilter::~InverseFilter()
{
}

void InverseFilter::processInternal()
{
	filteredParameter->setValue(jmap<float>(sourceParam->getNormalizedValue(), sourceParam->maximumValue, sourceParam->minimumValue));
}
