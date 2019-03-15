/*
  ==============================================================================

    NumberComparators.cpp
    Created: 2 Nov 2016 8:57:34pm
    Author:  bkupe

  ==============================================================================
*/

#include "NumberComparators.h"

FloatComparator::FloatComparator(Controllable * c) :
	ParameterComparator(c),
	floatParam((FloatParameter *)c)
{
	floatRef = addFloatParameter("Reference", "Comparison Reference to check against source value", floatParam->floatValue(), floatParam->minimumValue, floatParam->maximumValue);
	floatRef->defaultUI = floatParam->defaultUI;
	reference = floatRef;
	floatRef->setValue(floatParam->floatValue(), false, true, true);

	addCompareOption("=", equalsId);
	addCompareOption("!=", equalsId);
	addCompareOption(">", greaterId);
	addCompareOption("<", lessId);
	addCompareOption(">=", greaterOrEqualId);
	addCompareOption("<=", lessOrEqualId);
	addCompareOption("~", inRangeId);
	
}

FloatComparator::~FloatComparator()
{
}

void FloatComparator::compare()
{
	if (reference == nullptr) return;

	if (currentFunctionId == equalsId)				setValid(floatParam->floatValue() == floatRef->floatValue());
	else if (currentFunctionId == differentId)		setValid(floatParam->floatValue() != floatRef->floatValue());
	else if (currentFunctionId == greaterId)		setValid(floatParam->floatValue() > floatRef->floatValue());
	else if (currentFunctionId == lessId)			setValid(floatParam->floatValue() < floatRef->floatValue());
	else if (currentFunctionId == greaterOrEqualId)	setValid(floatParam->floatValue() >= floatRef->floatValue());
	else if (currentFunctionId == lessOrEqualId)	setValid(floatParam->floatValue() <= floatRef->floatValue());
	else if (currentFunctionId == inRangeId)		setValid(false); //not implemented, need RangeParameter
}


IntComparator::IntComparator(Controllable * c) :
	ParameterComparator(c),
	intParam((IntParameter *)c)
{
	

	intRef = addIntParameter("Reference", "Comparison Reference to check against source value", intParam->defaultValue,intParam->minimumValue,intParam->maximumValue);
	reference = intRef;

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

void IntComparator::compare()
{
	if (currentFunctionId == equalsId)				setValid(intParam->floatValue() == intRef->floatValue());
	if (currentFunctionId == differentId)			setValid(intParam->floatValue() != intRef->floatValue());
	else if (currentFunctionId == greaterId)		setValid(intParam->floatValue() > intRef->floatValue());
	else if (currentFunctionId == lessId)			setValid(intParam->floatValue() < intRef->floatValue());
	else if (currentFunctionId == greaterOrEqualId)	setValid(intParam->floatValue() >= intRef->floatValue());
	else if (currentFunctionId == lessOrEqualId)	setValid(intParam->floatValue() <= intRef->floatValue());
	else if (currentFunctionId == inRangeId)		setValid(false); //not implemented, need RangeParameter
}

