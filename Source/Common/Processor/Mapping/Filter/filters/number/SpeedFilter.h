/*
  ==============================================================================

    SpeedFilter.h
    Created: 2 Nov 2021 3:11:00pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class SpeedFilter :
	public TimeFilter
{
public:
	SpeedFilter(var params, Multiplex* multiplex);
	~SpeedFilter();

	HashMap<Parameter*, var> prevValueMap;
	HashMap<Parameter*, var> speedMap;
	IntParameter* smoothing;

	void setupParametersInternal(int multiplexIndex, bool rangeOnly) override;
	Parameter* setupSingleParameterInternal(Parameter* source, int multiplexIndex, bool rangeOnly) override;
	virtual ProcessResult processSingleParameterTimeInternal(Parameter* source, Parameter* out, int multiplexIndex, double deltaTime) override;

	void filterParamChanged(Parameter* c) override;

	String getTypeString() const override { return "Speed"; }
};
