/*
  ==============================================================================

    SimpleSmoothFilter.h
    Created: 4 Feb 2017 5:39:34pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

class SimpleSmoothFilter :
	public MappingFilter
{
public:
	SimpleSmoothFilter(var params, Multiplex* multiplex);
	~SimpleSmoothFilter();

	BoolParameter * async;
	FloatParameter * smooth;
	FloatParameter * downSmooth;

	const float precision = .00001f;

	Parameter* setupSingleParameterInternal(Parameter* source, int multiplexIndex, bool rangeOnly) override;
	ProcessResult processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex) override;

	void onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c) override;

	String getTypeString() const override { return "Smooth"; }
};
