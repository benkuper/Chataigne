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
	InverseFilter(var params, Multiplex* multiplex);
	~InverseFilter(); 

	ProcessResult processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex) override;
	
	virtual String getTypeString() const override { return "Inverse"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InverseFilter)
};
