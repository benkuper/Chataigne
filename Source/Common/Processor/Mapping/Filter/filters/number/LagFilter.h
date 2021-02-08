/*
  ==============================================================================

    LagFilter.h
    Created: 4 Feb 2017 5:39:47pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../../MappingFilter.h"



class LagFilter :
	public MappingFilter,
	public Timer
{
public:
	LagFilter(var params, Multiplex* multiplex);
	~LagFilter();

	HashMap<Parameter*, var> paramTempValueMap;
	FloatParameter * frequency;

	void setupParametersInternal(int multiplexIndex) override;
	Parameter * setupSingleParameterInternal(Parameter * source, int multiplexIndex) override;
	bool processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex) override;

	void filterParamChanged(Parameter * p) override;

	String getTypeString() const override { return "FPS"; }

	virtual void timerCallback() override;

};