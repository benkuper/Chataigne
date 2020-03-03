/*
  ==============================================================================

    InverseFilter.h
    Created: 4 Feb 2017 5:39:15pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../../MappingFilter.h"


class InverseFilter :
	public MappingFilter
{
public:
	InverseFilter(var params);
	~InverseFilter(); 

	void processSingleParameterInternal(Parameter * source, Parameter * out) override;
	
	static InverseFilter * create(var params) { return new InverseFilter(params); }
	virtual String getTypeString() const override { return "Inverse"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InverseFilter)
};
