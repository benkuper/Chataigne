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
	SimpleRemapFilter();
	~SimpleRemapFilter();

	Parameter * targetMin;
	Parameter * targetMax;

	void processInternal() override;

	Parameter * setupParameterInternal(Parameter *) override;
	void filterParamChanged(Parameter *) override;

	static SimpleRemapFilter * create() { return new SimpleRemapFilter(); }
	String getTypeString() const override { return "Simple Remap"; }
};



#endif  // SIMPLEREMAPFILTER_H_INCLUDED
