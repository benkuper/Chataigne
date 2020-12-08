/*
  ==============================================================================

    SimpleSmoothFilter.h
    Created: 4 Feb 2017 5:39:34pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../../MappingFilter.h"

class SimpleSmoothFilter :
	public MappingFilter
{
public:
	SimpleSmoothFilter(var params);
	~SimpleSmoothFilter();

	BoolParameter * async;
	FloatParameter * smooth;
	FloatParameter * downSmooth;

	const float precision = .00001f;

	Parameter* setupSingleParameterInternal(Parameter* source) override;
	bool processSingleParameterInternal(Parameter * source, Parameter * out) override;

	void onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c) override;

	String getTypeString() const override { return "Smooth"; }
};
