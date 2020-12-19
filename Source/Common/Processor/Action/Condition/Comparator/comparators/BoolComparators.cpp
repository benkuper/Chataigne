/*
  ==============================================================================

    BoolComparators.cpp
    Created: 2 Nov 2016 8:58:50pm
    Author:  bkupe

  ==============================================================================
*/

#include "BoolComparators.h"

BoolComparator::BoolComparator(Array<WeakReference<Controllable>> sources) :
	ParameterComparator(sources)
{
	boolParams.addArray(sources);

	boolRef = addBoolParameter("Reference", "Comparison Reference to check against source value", boolParams[0]->boolValue());
	boolRef->setValue(boolParams[0]->boolValue(), false, true, true);
	reference = boolRef;

	addCompareOption("=", equalsId);
	addCompareOption("!=", differentId);

}

BoolComparator::~BoolComparator()
{
}

void BoolComparator::compare(int iterationIndex)
{
	if (currentFunctionId == equalsId) setValid(iterationIndex, boolParams[iterationIndex]->boolValue() == boolRef->boolValue());
	else if (currentFunctionId == differentId) setValid(iterationIndex, boolParams[iterationIndex]->boolValue() != boolRef->boolValue()); 
}