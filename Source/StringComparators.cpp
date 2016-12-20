/*
  ==============================================================================

    StringComparators.cpp
    Created: 2 Nov 2016 8:58:35pm
    Author:  bkupe

  ==============================================================================
*/

#include "StringComparators.h"


StringComparator::StringComparator(Controllable * c) :
	ParameterComparator(c),
	stringParam((StringParameter *)c)
{
	addCompareOption("=", equalsId);
	addCompareOption("Contains", containsId);
	addCompareOption("Starts with", startsWith);
	addCompareOption("Ends with", endsWidth);

	stringRef = addStringParameter("Reference", "Comparison Reference to check against source value", stringParam->defaultValue);
	reference = stringRef;
}

StringComparator::~StringComparator()
{
}

void StringComparator::compare()
{
	if (currentFunctionId == equalsId)				setValid(stringParam->stringValue() == stringRef->stringValue());
	else if (currentFunctionId == containsId)		setValid(stringParam->stringValue().contains(stringRef->stringValue()));
	else if (currentFunctionId == startsWith)		setValid(stringParam->stringValue().startsWith(stringRef->stringValue()));
	else if (currentFunctionId == endsWidth)		setValid(stringParam->stringValue().endsWith(stringRef->stringValue()));
}