/*
  ==============================================================================

    NumberComparators.h
    Created: 2 Nov 2016 8:57:34pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class NumberComparator :
	public BaseComparator
{
public:
	NumberComparator(Parameter * sourceParam, Multiplex* multiplex);
	virtual ~NumberComparator();

	const Identifier equalsId = "=";
	const Identifier differentId = "!=";
	const Identifier greaterId = ">";
	const Identifier lessId = "<";
	const Identifier greaterOrEqualId = ">=";
	const Identifier lessOrEqualId = "<=";
	const Identifier moduloEqualId = "%=";
	const Identifier moduloGreaterId = "%>";
	const Identifier moduloLessId = "%<";
	const Identifier floorEqualId = "f=";
	const Identifier floorGreaterId = "f>"; 
	const Identifier floorLessId = "f<"; 
	const Identifier diffGreaterId = "d>";
	const Identifier diffLessId = "d<";
	const Identifier inRangeId = "r";

	bool isFloat;
	Parameter * refParam;
	var sourceRange;
	Array<float> prevValues;

	virtual void compareFunctionChanged() override;
	virtual void setupReferenceParam();
	
	virtual bool compareInternal(Parameter* sourceParam, int multiplexIndex = 0) override;
};