/*
  ==============================================================================

    StringComparators.h
    Created: 2 Nov 2016 8:58:35pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../BaseComparator.h"

class StringComparator :
	public ParameterComparator
{
public:
	StringComparator(Array<WeakReference<Controllable>> sources);
	virtual ~StringComparator();

	const Identifier equalsId = "=";
	const Identifier differentId = "!=";
	const Identifier containsId = "contains";
	const Identifier startsWith = "startsWith";
	const Identifier endsWidth = "endsWidth";

	Array<StringParameter *> stringParams;
	StringParameter * stringRef;

	virtual void compare(int iterationIndex) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StringComparator)
};