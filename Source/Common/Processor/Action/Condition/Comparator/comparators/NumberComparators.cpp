/*
  ==============================================================================

    NumberComparators.cpp
    Created: 2 Nov 2016 8:57:34pm
    Author:  bkupe

  ==============================================================================
*/

#include "NumberComparators.h"

FloatComparator::FloatComparator(Array<WeakReference<Controllable>> sources) :
	ParameterComparator(sources)
{
	floatParams.addArray(sources);

	floatRef = addFloatParameter("Reference", "Comparison Reference to check against source value", floatParams[0]->floatValue(), floatParams[0]->minimumValue, floatParams[0]->maximumValue);
	floatRef->defaultUI = floatParams[0]->defaultUI;
	reference = floatRef;
	floatRef->setValue(floatParams[0]->floatValue(), false, true, true);

	addCompareOption("=", equalsId);
	addCompareOption("!=", differentId);
	addCompareOption(">", greaterId);
	addCompareOption("<", lessId);
	addCompareOption(">=", greaterOrEqualId);
	addCompareOption("<=", lessOrEqualId);
	addCompareOption("~", inRangeId);
	
}

FloatComparator::~FloatComparator()
{
}

void FloatComparator::compare(int iterationIndex)
{
	if (reference == nullptr) return;

	if (currentFunctionId == equalsId)				setValid(iterationIndex, floatParams[iterationIndex]->floatValue() == floatRef->floatValue());
	else if (currentFunctionId == differentId)		setValid(iterationIndex, floatParams[iterationIndex]->floatValue() != floatRef->floatValue());
	else if (currentFunctionId == greaterId)		setValid(iterationIndex, floatParams[iterationIndex]->floatValue() > floatRef->floatValue());
	else if (currentFunctionId == lessId)			setValid(iterationIndex, floatParams[iterationIndex]->floatValue() < floatRef->floatValue());
	else if (currentFunctionId == greaterOrEqualId)	setValid(iterationIndex, floatParams[iterationIndex]->floatValue() >= floatRef->floatValue());
	else if (currentFunctionId == lessOrEqualId)	setValid(iterationIndex, floatParams[iterationIndex]->floatValue() <= floatRef->floatValue());
	else if (currentFunctionId == inRangeId)		setValid(iterationIndex, false); //not implemented, need RangeParameter
}


IntComparator::IntComparator(Array<WeakReference<Controllable>> sources) :
	ParameterComparator(sources)
{
	intParams.addArray(sources);

	intRef = addIntParameter("Reference", "Comparison Reference to check against source value", intParams[0]->defaultValue, intParams[0]->minimumValue, intParams[0]->maximumValue);
	reference = intRef;
	intRef->setValue(intRef->floatValue(), false, true, true);

	addCompareOption("=", equalsId);
	addCompareOption("!=", differentId);
	addCompareOption(">", greaterId);
	addCompareOption("<", lessId);
	addCompareOption(">=", greaterOrEqualId);
	addCompareOption("<=", lessOrEqualId);
	addCompareOption("~", inRangeId);
}

IntComparator::~IntComparator()
{
}

void IntComparator::compare(int iterationIndex)
{
	if (currentFunctionId == equalsId)				setValid(iterationIndex, intParams[iterationIndex]->floatValue() == intRef->floatValue());
	if (currentFunctionId == differentId)			setValid(iterationIndex, intParams[iterationIndex]->floatValue() != intRef->floatValue());
	else if (currentFunctionId == greaterId)		setValid(iterationIndex, intParams[iterationIndex]->floatValue() > intRef->floatValue());
	else if (currentFunctionId == lessId)			setValid(iterationIndex, intParams[iterationIndex]->floatValue() < intRef->floatValue());
	else if (currentFunctionId == greaterOrEqualId)	setValid(iterationIndex, intParams[iterationIndex]->floatValue() >= intRef->floatValue());
	else if (currentFunctionId == lessOrEqualId)	setValid(iterationIndex, intParams[iterationIndex]->floatValue() <= intRef->floatValue());
	else if (currentFunctionId == inRangeId)		setValid(iterationIndex, false); //not implemented, need RangeParameter
}

