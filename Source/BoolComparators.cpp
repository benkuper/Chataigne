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
	addCompareOption("=", equalsId);

	ref = addBoolParameter("Reference", "Comparison Reference to check against source value", boolParam->defaultValue);
}

BoolComparator::~BoolComparator()
{
}

void BoolComparator::compare()
{
	if (currentFunctionId == equalsId) setValid(boolParam->boolValue() > ref->boolValue());
}