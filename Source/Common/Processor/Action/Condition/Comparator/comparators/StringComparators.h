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
	StringComparator(Controllable *c);
	virtual ~StringComparator();

	const Identifier equalsId = "=";
	const Identifier differentId = "!=";
	const Identifier containsId = "contains";
	const Identifier startsWith = "startsWith";
	const Identifier endsWidth = "endsWidth";

	StringParameter * stringParam;
	StringParameter * stringRef;

	virtual void compare() override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StringComparator)
};