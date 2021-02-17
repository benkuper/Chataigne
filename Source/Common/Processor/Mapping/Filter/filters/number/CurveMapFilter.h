/*
  ==============================================================================

    CurveMapFilter.h
    Created: 4 Feb 2017 5:39:24pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "SimpleRemapFilter.h"

class CurveMapFilter :
	public SimpleRemapFilter
{
public:
	CurveMapFilter(var params, Multiplex* multiplex);
	~CurveMapFilter();

	Automation curve;


	ProcessResult processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex) override;

	void onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c) override;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	//InspectableEditor * getEditor(bool isRoot) override;

	String getTypeString() const override { return "Curve Map"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CurveMapFilter)
};
