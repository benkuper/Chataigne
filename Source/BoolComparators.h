/*
  ==============================================================================

    BoolComparators.h
    Created: 2 Nov 2016 8:58:50pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef BOOLCOMPARATORS_H_INCLUDED
#define BOOLCOMPARATORS_H_INCLUDED

#include "BaseComparator.h"

class BoolComparator :
	public ParameterComparator
{
public:
	BoolComparator(Controllable * c);
	virtual ~BoolComparator();

	const Identifier equalsId = "=";

	BoolParameter * boolParam;
	BoolParameter * ref;

	virtual void compare() override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BoolComparator)
};



#endif  // BOOLCOMPARATORS_H_INCLUDED
