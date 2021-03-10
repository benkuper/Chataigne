/*
  ==============================================================================

    BoolComparators.h
    Created: 2 Nov 2016 8:58:50pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class BoolComparator :
	public BaseComparator
{
public:
	BoolComparator(Parameter * sourceParam, Multiplex * multiplex);
	virtual ~BoolComparator();

	const Identifier equalsId = "=";
	const Identifier differentId = "!=";

	virtual bool compare(Parameter* sourceParam, int multiplexIndex = 0) override;
};