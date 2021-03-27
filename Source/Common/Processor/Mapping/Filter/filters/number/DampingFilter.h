/*
  ==============================================================================

    DampingFilter.h
    Created: 8 Dec 2020 11:36:15am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class DampingFilter :
	public MappingFilter
{
public:
	DampingFilter(var params, Multiplex* multiplex);
	~DampingFilter();

	FloatParameter* force;
	FloatParameter* friction;

	HashMap<Parameter *, var> previousSpeedsMap;

	double timeAtLastUpdate;
	const float precision = .00001f;

	void setupParametersInternal(int multiplexIndex, bool rangeOnly) override;
	Parameter* setupSingleParameterInternal(Parameter* source, int multiplexIndex, bool rangeOnly) override;
	ProcessResult processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex) override;

	String getTypeString() const override { return "Damping"; }
};
