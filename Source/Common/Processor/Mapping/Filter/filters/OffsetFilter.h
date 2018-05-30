/*
  ==============================================================================

    OffsetFilter.h
    Created: 19 Jan 2018 11:20:10am
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../MappingFilter.h"

class OffsetFilter :
	public MappingFilter
{
public:
	OffsetFilter(var params);
	~OffsetFilter();

	FloatParameter * offset;

	void processInternal() override;

	static OffsetFilter * create(var params) { return new OffsetFilter(params); }
	virtual String getTypeString() const override { return "Offset"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OffsetFilter)
};
