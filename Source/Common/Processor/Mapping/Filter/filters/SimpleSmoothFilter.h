/*
  ==============================================================================

    SimpleSmoothFilter.h
    Created: 4 Feb 2017 5:39:34pm
    Author:  Ben

  ==============================================================================
*/

#ifndef SIMPLESMOOTHFILTER_H_INCLUDED
#define SIMPLESMOOTHFILTER_H_INCLUDED

#include "../MappingFilter.h"

class SimpleSmoothFilter :
	public MappingFilter
{
public:
	SimpleSmoothFilter(var params);
	~SimpleSmoothFilter();

	FloatParameter * smooth;

	void processInternal() override;


	static SimpleSmoothFilter * create(var params) { return new SimpleSmoothFilter(params); }
	String getTypeString() const override { return "Smooth"; }

};



#endif  // SIMPLESMOOTHFILTER_H_INCLUDED
