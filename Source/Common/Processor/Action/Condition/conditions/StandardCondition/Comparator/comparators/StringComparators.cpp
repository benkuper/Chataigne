/*
  ==============================================================================

    StringComparators.cpp
    Created: 2 Nov 2016 8:58:35pm
    Author:  bkupe

  ==============================================================================
*/

#include "StringComparators.h"


StringComparator::StringComparator(Parameter *sourceParam)
{
	reference = addStringParameter("Reference", "Comparison Reference to check against source value", sourceParam->stringValue());
	reference->setValue(sourceParam->stringValue(), false, true, true);

	addCompareOption("=", equalsId);
	addCompareOption("!=", differentId);
	addCompareOption("Contains", containsId);
	addCompareOption("Starts with", startsWith);
	addCompareOption("Ends with", endsWidth);
}

StringComparator::~StringComparator()
{
}

bool StringComparator::compare(Parameter *sourceParam)
{
	if (currentFunctionId == equalsId)				return sourceParam->stringValue() == reference->stringValue();
	else if (currentFunctionId == differentId)		return sourceParam->stringValue() != reference->stringValue();
	else if (currentFunctionId == containsId)		return sourceParam->stringValue().contains(reference->stringValue());
	else if (currentFunctionId == startsWith)		return sourceParam->stringValue().startsWith(reference->stringValue());
	else if (currentFunctionId == endsWidth)		return sourceParam->stringValue().endsWith(reference->stringValue());
}