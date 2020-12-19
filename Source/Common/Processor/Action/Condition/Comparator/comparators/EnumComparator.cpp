/*
  ==============================================================================

    EnumComparator.cpp
    Created: 19 Feb 2017 5:48:15pm
    Author:  Ben

  ==============================================================================
*/

#include "EnumComparator.h"


EnumComparator::EnumComparator(Array<WeakReference<Controllable>> sources) :
	ParameterComparator(sources)
{
	enumParams.addArray(sources);

	enumRef = addEnumParameter("Reference", "Comparison Reference to check against source value");
	
	for (auto &ev : enumParams[0]->enumValues) enumRef->addOption(ev->key, ev->value);

	addCompareOption("=", equalsId);
	addCompareOption("!=", equalsId);

	enumRef->setValue(enumParams[0]->value, false, true, true);

	reference = enumRef;

}

EnumComparator::~EnumComparator()
{
}

void EnumComparator::compare(int iterationIndex)
{
	if (currentFunctionId == equalsId) setValid(iterationIndex, enumParams[iterationIndex]->getValueData() == enumRef->getValueData());
	if (currentFunctionId == differentId) setValid(iterationIndex, enumParams[iterationIndex]->getValueData() != enumRef->getValueData());
}
