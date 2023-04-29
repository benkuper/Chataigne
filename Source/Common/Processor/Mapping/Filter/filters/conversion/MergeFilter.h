/*
  ==============================================================================

    MergeFilter.h
    Created: 21 Sep 2021 9:22:16am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class MergeFilter :
	public MappingFilter
{
public:
	MergeFilter(var params, Multiplex* multiplex);
	~MergeFilter();

	enum Operator {MIN, MAX, AVERAGE, SUM, MULTIPLY, DISTANCE, DIFF };
	EnumParameter* op;

	void setupParametersInternal(int multiplexIndex, bool rangeOnly) override;
	ProcessResult processInternal(Array<Parameter*> inputs, int multiplexIndex) override; 
	
	String getTypeString() const override { return "Merge"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MergeFilter)
};