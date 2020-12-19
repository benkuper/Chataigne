/*
  ==============================================================================

    EnumComparator.h
    Created: 19 Feb 2017 5:48:15pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../BaseComparator.h"

class EnumComparator :
	public ParameterComparator
{
public:
	EnumComparator(Array<WeakReference<Controllable>> sources);
	virtual ~EnumComparator();

	const Identifier equalsId = "=";
	const Identifier differentId = "!=";

	Array<EnumParameter *> enumParams;
	EnumParameter * enumRef;

	virtual void compare(int iterationIndex) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnumComparator)
};