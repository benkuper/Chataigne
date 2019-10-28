/*
  ==============================================================================

    BoolComparators.h
    Created: 2 Nov 2016 8:58:50pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../BaseComparator.h"

class BoolComparator :
	public ParameterComparator
{
public:
	BoolComparator(Controllable * c);
	virtual ~BoolComparator();

	const Identifier equalsId = "=";
	const Identifier differentId = "!=";

	BoolParameter * boolParam;
	BoolParameter * boolRef;

	virtual void compare() override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BoolComparator)
};