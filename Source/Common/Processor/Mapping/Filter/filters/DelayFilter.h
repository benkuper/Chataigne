/*
  ==============================================================================

    DelayFilter.h
    Created: 12 Oct 2021 10:17:44am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class DelayFilter :
	public MappingFilter
{
public:
	DelayFilter(var params, Multiplex* multiplex);
	~DelayFilter();

	struct TimeValue
	{
		double time;
		var val;
	};

	HashMap<WeakReference<Parameter>, Array<TimeValue>> paramTimeValueMap;
	FloatParameter* delay;

	void multiplexCountChanged() override;
	void setupParametersInternal(int multiplexIndex, bool rangeOnly) override;
	Parameter* setupSingleParameterInternal(Parameter* source, int multiplexIndex, bool rangeOnly) override;
	ProcessResult processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex) override;

	void filterParamChanged(Parameter* p) override;

	String getTypeString() const override { return "Delay"; }
};