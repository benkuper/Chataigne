/*
  ==============================================================================

    SimpleRemapFilter.h
    Created: 4 Feb 2017 5:39:07pm
    Author:  Ben

  ==============================================================================
*/

#ifndef SIMPLEREMAPFILTER_H_INCLUDED
#define SIMPLEREMAPFILTER_H_INCLUDED

#include "MappingFilter.h"

class SimpleRemapFilter :
	public MappingFilter
{
public:
	SimpleRemapFilter(var params);
	~SimpleRemapFilter();

	Parameter * targetMin;
	Parameter * targetMax;

	void processInternal() override;

	void filterParamChanged(Parameter *) override;

	Parameter * setupParameterInternal(Parameter * source) override;

	static SimpleRemapFilter * create(var params) { return new SimpleRemapFilter(params); }
	String getTypeString() const override { return "Simple Remap"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleRemapFilter)
};



#endif  // SIMPLEREMAPFILTER_H_INCLUDED
