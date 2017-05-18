/*
  ==============================================================================

    EnumComparator.h
    Created: 19 Feb 2017 5:48:15pm
    Author:  Ben

  ==============================================================================
*/

#ifndef ENUMCOMPARATOR_H_INCLUDED
#define ENUMCOMPARATOR_H_INCLUDED


#include "BaseComparator.h"

class EnumComparator :
	public ParameterComparator
{
public:
	EnumComparator(Controllable * c);
	virtual ~EnumComparator();

	const Identifier equalsId = "=";
	const Identifier differentId = "!=";

	EnumParameter * enumParam;
	EnumParameter * enumRef;

	virtual void compare() override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnumComparator)
};




#endif  // ENUMCOMPARATOR_H_INCLUDED
