/*
  ==============================================================================

    BoolComparators.cpp
    Created: 2 Nov 2016 8:58:50pm
    Author:  bkupe

  ==============================================================================
*/

#include "BoolComparators.h"

BoolComparator::BoolComparator(Parameter * sourceParam, Multiplex* multiplex) :
	BaseComparator(multiplex)
{
	setReferenceParam(new BoolParameter("Reference", "Comparison Reference to check against source value", sourceParam->boolValue()));
	reference->setValue(sourceParam->boolValue(), false, true, true);

	addCompareOption("=", equalsId);
	addCompareOption("!=", differentId);
}

BoolComparator::~BoolComparator()
{
}

bool BoolComparator::compare(Parameter * sourceParam, int multiplexIndex)
{
	bool value = isMultiplexed() ? refLink->getLinkedValue(multiplexIndex) : reference->boolValue();
	if (currentFunctionId == equalsId) return sourceParam->boolValue() == value;
	else if (currentFunctionId == differentId) return sourceParam->boolValue() != reference->boolValue();
	return false;
}