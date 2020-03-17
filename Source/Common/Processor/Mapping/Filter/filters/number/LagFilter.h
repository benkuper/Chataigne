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
	LagFilter(var params);
	~LagFilter();

	HashMap<Parameter*, var> paramTempValueMap;
	FloatParameter * frequency;

	void setupParametersInternal() override;
	Parameter * setupSingleParameterInternal(Parameter * source) override;
	void processSingleParameterInternal(Parameter * source, Parameter * out) override;

	void filterParamChanged(Parameter * p) override;

	static LagFilter * create(var params) { return new LagFilter(params); }
	String getTypeString() const override { return "FPS"; }

	virtual void timerCallback() override;

};