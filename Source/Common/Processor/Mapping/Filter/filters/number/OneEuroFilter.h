/*
  ==============================================================================

	OneEuroFilter.h
	Created: 18 Jun 2021 10:45:02am
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class OneEuroMappingFilter :
	public TimeFilter
{
public:
	OneEuroMappingFilter(var params, Multiplex* multiplex);
	~OneEuroMappingFilter();


	FloatParameter* beta_;
	FloatParameter* mincutoff;

	double freq;
	double dcutoff;

	HashMap<Parameter*, OneEuroFilter*> filtersMap;
	OwnedArray<OneEuroFilter> allFilters;

	double alpha(double cutoff);

	virtual void setupParametersInternal(int multiplexIndex, bool rangeOnly = false) override;
	Parameter* setupSingleParameterInternal(Parameter* source, int multiplexIndex, bool rangeOnly) override;
	ProcessResult processSingleParameterTimeInternal(Parameter* source, Parameter* out, int multiplexIndex, double deltaTime) override;

	void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;

	String getTypeString() const override { return "One Euro"; }
};