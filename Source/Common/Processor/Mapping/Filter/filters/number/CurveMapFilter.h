/*
  ==============================================================================

    CurveMapFilter.h
    Created: 4 Feb 2017 5:39:24pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../../MappingFilter.h"

class CurveMapFilter :
	public MappingFilter
{
public:
	CurveMapFilter(var params);
	~CurveMapFilter();

	Automation curve;

	void processSingleParameterInternal(Parameter* source, Parameter* out) override;
	
	void onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c) override;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	//InspectableEditor * getEditor(bool isRoot) override;

	static CurveMapFilter * create(var params) { return new CurveMapFilter(params); }
	String getTypeString() const override { return "Curve Map"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CurveMapFilter)
};
