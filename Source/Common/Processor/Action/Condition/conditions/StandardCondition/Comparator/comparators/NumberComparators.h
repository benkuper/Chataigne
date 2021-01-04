/*
  ==============================================================================

    NumberComparators.h
    Created: 2 Nov 2016 8:57:34pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../BaseComparator.h"

class NumberComparator :
	public BaseComparator
{
public:
	NumberComparator(Parameter * sourceParam);
	virtual ~NumberComparator();

	const Identifier equalsId = "=";
	const Identifier differentId = "!=";
	const Identifier greaterId = ">";
	const Identifier lessId = "<";
	const Identifier greaterOrEqualId = ">=";
	const Identifier lessOrEqualId = "<=";
	const Identifier inRangeId = "range";

	Parameter * refParam;

	virtual bool compare(Parameter * sourceParam) override;
};