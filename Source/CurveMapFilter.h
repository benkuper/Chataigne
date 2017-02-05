/*
  ==============================================================================

    CurveMapFilter.h
    Created: 4 Feb 2017 5:39:24pm
    Author:  Ben

  ==============================================================================
*/

#ifndef CURVEMAPFILTER_H_INCLUDED
#define CURVEMAPFILTER_H_INCLUDED

#include "MappingFilter.h"
#include "Automation.h"

class CurveMapFilter :
	public MappingFilter
{
public:
	CurveMapFilter();
	~CurveMapFilter();

	Automation curve;

	void processInternal() override;


	static CurveMapFilter * create() { return new CurveMapFilter(); }
	String getTypeString() const override { return "Curve Map"; }
};





#endif  // CURVEMAPFILTER_H_INCLUDED
