/*
  ==============================================================================

    EnumComparator.cpp
    Created: 19 Feb 2017 5:48:15pm
    Author:  Ben

  ==============================================================================
*/

#include "EnumComparator.h"


EnumComparator::EnumComparator(Parameter * sourceParam, Multiplex* multiplex) :
	BaseComparator(multiplex)
{
	EnumParameter* ep = (EnumParameter *)sourceParam;

	setReferenceParam(new EnumParameter("Reference", "Comparison Reference to check against source value"));
	enumRef = (EnumParameter*)reference;

	for (auto &ev : ep->enumValues) enumRef->addOption(ev->key, ev->value);

	addCompareOption("=", equalsId);
	addCompareOption("!=", differentId);

	enumRef->setValue(ep->value, false, true, true);
}

EnumComparator::~EnumComparator()
{
}

bool EnumComparator::compare(Parameter* sourceParam, int multiplexIndex)
{
	var value = isMultiplexed() ? refLink->getLinkedValue(multiplexIndex) : enumRef->getValueData();
	if (currentFunctionId == equalsId) return ((EnumParameter*)sourceParam)->getValueData() == value;
	if (currentFunctionId == differentId) return ((EnumParameter *)sourceParam)->getValueData() != value;
	return false;
}
