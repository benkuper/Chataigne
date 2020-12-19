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
	BoolComparator(Array<WeakReference<Controllable>> sources);
	virtual ~BoolComparator();

	const Identifier equalsId = "=";
	const Identifier differentId = "!=";

	Array<BoolParameter *> boolParams;
	BoolParameter * boolRef;

	virtual void compare(int iterationIndex) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BoolComparator)
};