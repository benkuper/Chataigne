/*
  ==============================================================================

    BoolComparators.cpp
    Created: 2 Nov 2016 8:58:50pm
    Author:  bkupe

  ==============================================================================
*/

#include "BoolComparators.h"

BoolComparator::BoolComparator(Controllable * c) :
	ParameterComparator(c),
	boolParam((BoolParameter *)c)
{
	
	boolRef = addBoolParameter("Reference", "Comparison Reference to check against source value", boolParam->boolValue());
	boolRef->setValue(boolParam->boolValue(), false, true, true);
	reference = boolRef;

	addCompareOption("=", equalsId);
	addCompareOption("!=", differentId);

}

BoolComparator::~BoolComparator()
{
}

void BoolComparator::compare()
{
	if (currentFunctionId == equalsId) setValid(boolParam->boolValue() == boolRef->boolValue());
	else if (currentFunctionId == differentId) setValid(boolParam->boolValue() != boolRef->boolValue()); 
}