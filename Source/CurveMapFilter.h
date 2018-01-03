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
	CurveMapFilter(var params);
	~CurveMapFilter();

	Automation curve;

	void processInternal() override;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	Parameter * setupParameterInternal(Parameter *, const String &forceType) override;

	//InspectableEditor * getEditor(bool isRoot) override;

	static CurveMapFilter * create(var params) { return new CurveMapFilter(params); }
	String getTypeString() const override { return "Curve Map"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CurveMapFilter)
};





#endif  // CURVEMAPFILTER_H_INCLUDED
