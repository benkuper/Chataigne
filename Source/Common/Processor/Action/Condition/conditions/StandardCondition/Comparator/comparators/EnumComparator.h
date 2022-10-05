/*
  ==============================================================================

    EnumComparator.h
    Created: 19 Feb 2017 5:48:15pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

class EnumComparator :
	public BaseComparator
{
public:
	EnumComparator(Parameter * sourceParam, Multiplex* multiplex);
	virtual ~EnumComparator();

	const Identifier equalsId = "=";
	const Identifier differentId = "!=";
	const Identifier changeId = "ch";

	EnumParameter * enumRef;

	virtual bool compareInternal(Parameter* sourceParam, int multiplexIndex = 0) override;
};