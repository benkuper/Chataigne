/*
  ==============================================================================

    EnumComparator.cpp
    Created: 19 Feb 2017 5:48:15pm
    Author:  Ben

  ==============================================================================
*/

#include "EnumComparator.h"


EnumComparator::EnumComparator(Parameter * sourceParam)
{
	EnumParameter* ep = (EnumParameter *)sourceParam;
	enumRef = addEnumParameter("Reference", "Comparison Reference to check against source value");
	for (auto &ev : ep->enumValues) enumRef->addOption(ev->key, ev->value);

	addCompareOption("=", equalsId);
	addCompareOption("!=", differentId);

	enumRef->setValue(ep->value, false, true, true);

	reference = enumRef;
}

EnumComparator::~EnumComparator()
{
}

bool EnumComparator::compare(Parameter* sourceParam)
{
	if (currentFunctionId == equalsId) return ((EnumParameter*)sourceParam)->getValueData() == enumRef->getValueData();
	if (currentFunctionId == differentId) return ((EnumParameter *)sourceParam)->getValueData() != enumRef->getValueData();
	return false;
}
