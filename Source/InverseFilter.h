/*
  ==============================================================================

    InverseFilter.h
    Created: 4 Feb 2017 5:39:15pm
    Author:  Ben

  ==============================================================================
*/

#ifndef INVERSEFILTER_H_INCLUDED
#define INVERSEFILTER_H_INCLUDED

#include "MappingFilter.h"

class InverseFilter :
	public MappingFilter
{
public:
	InverseFilter();
	~InverseFilter();

	void processInternal(Parameter * source, Parameter * target) override;
	
	static InverseFilter * create() { return new InverseFilter(); }
	virtual String getTypeString() const override { return "Inverse"; }
};



#endif  // INVERSEFILTER_H_INCLUDED
