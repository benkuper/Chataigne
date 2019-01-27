/*
  ==============================================================================

    EnumComparator.cpp
    Created: 19 Feb 2017 5:48:15pm
    Author:  Ben

  ==============================================================================
*/

#include "EnumComparator.h"


EnumComparator::EnumComparator(Controllable * c) :
	ParameterComparator(c),
	enumParam((EnumParameter *)c)
{
	
	enumRef = addEnumParameter("Reference", "Comparison Reference to check against source value");
	
	for (auto &ev : enumParam->enumValues) enumRef->addOption(ev->key, ev->value);

	addCompareOption("=", equalsId);
	addCompareOption("!=", equalsId);

	reference = enumRef;

}

EnumComparator::~EnumComparator()
{
}

void EnumComparator::compare()
{
	if (currentFunctionId == equalsId) setValid(enumParam->getValueData() == enumRef->getValueData());
	if (currentFunctionId == differentId) setValid(enumParam->getValueData() != enumRef->getValueData());
}
