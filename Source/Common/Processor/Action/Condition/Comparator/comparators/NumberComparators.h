/*
  ==============================================================================

    NumberComparators.h
    Created: 2 Nov 2016 8:57:34pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../BaseComparator.h"

class FloatComparatorUI;

class FloatComparator :
	public ParameterComparator
{
public:
	FloatComparator(Controllable * c);
	virtual ~FloatComparator();

	const Identifier equalsId = "=";
	const Identifier differentId = "!=";
	const Identifier greaterId = ">";
	const Identifier lessId = "<";
	const Identifier greaterOrEqualId = ">=";
	const Identifier lessOrEqualId = "<=";
	const Identifier inRangeId = "range";

	FloatParameter * floatParam;
	FloatParameter * floatRef;

	virtual void compare() override;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FloatComparator)
};


class IntComparator :
	public ParameterComparator
{
public:
	IntComparator(Controllable *c);
	virtual ~IntComparator();

	const Identifier equalsId = "=";
	const Identifier differentId = "!=";
	const Identifier greaterId = ">";
	const Identifier lessId = "<";
	const Identifier greaterOrEqualId = ">=";
	const Identifier lessOrEqualId = "<=";
	const Identifier inRangeId = "range";

	IntParameter * intParam;
	IntParameter *  intRef;

	virtual void compare() override; 


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IntComparator)
};