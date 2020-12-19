/*
  ==============================================================================

    StringComparators.cpp
    Created: 2 Nov 2016 8:58:35pm
    Author:  bkupe

  ==============================================================================
*/

#include "StringComparators.h"


StringComparator::StringComparator(Array<WeakReference<Controllable>> sources) :
	ParameterComparator(sources)
{
	stringParams.addArray(sources);

	stringRef = addStringParameter("Reference", "Comparison Reference to check against source value", stringParams[0]->stringValue());
	stringRef->setValue(stringParams[0]->stringValue(), false, true, true);
	reference = stringRef;

	addCompareOption("=", equalsId);
	addCompareOption("!=", differentId);
	addCompareOption("Contains", containsId);
	addCompareOption("Starts with", startsWith);
	addCompareOption("Ends with", endsWidth);
}

StringComparator::~StringComparator()
{
}

void StringComparator::compare(int iterationIndex)
{
	if (currentFunctionId == equalsId)				setValid(iterationIndex, stringParams[iterationIndex]->stringValue() == stringRef->stringValue());
	else if (currentFunctionId == differentId)		setValid(iterationIndex, stringParams[iterationIndex]->stringValue() != stringRef->stringValue());
	else if (currentFunctionId == containsId)		setValid(iterationIndex, stringParams[iterationIndex]->stringValue().contains(stringRef->stringValue()));
	else if (currentFunctionId == startsWith)		setValid(iterationIndex, stringParams[iterationIndex]->stringValue().startsWith(stringRef->stringValue()));
	else if (currentFunctionId == endsWidth)		setValid(iterationIndex, stringParams[iterationIndex]->stringValue().endsWith(stringRef->stringValue()));
}