/*
  ==============================================================================

    DampingFilter.h
    Created: 8 Dec 2020 11:36:15am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class DampingFilter :
	public TimeFilter
{
public:
	DampingFilter(var params, Multiplex* multiplex);
	~DampingFilter();

	FloatParameter* force;
	FloatParameter* friction;

	HashMap<Parameter *, var> previousSpeedsMap;

	Array<double> timesAtLastUpdate; //multiplexed
	Array<double> deltaTimes; //multiplexed
	const float precision = .00001f;

	void setupParametersInternal(int multiplexIndex, bool rangeOnly) override;
	Parameter* setupSingleParameterInternal(Parameter* source, int multiplexIndex, bool rangeOnly) override;
	virtual ProcessResult processSingleParameterTimeInternal(Parameter* source, Parameter* out, int multiplexIndex, double deltaTime) override;

	String getTypeString() const override { return "Damping"; }
};
