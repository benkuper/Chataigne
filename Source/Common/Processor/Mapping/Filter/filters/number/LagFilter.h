/*
  ==============================================================================

    LagFilter.h
    Created: 4 Feb 2017 5:39:47pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

class LagFilter :
	public MappingFilter
{
public:
	LagFilter(var params, Multiplex* multiplex);
	~LagFilter();

	HashMap<Parameter*, var> paramTempValueMap;
	Array<double> lastSendTime;
	FloatParameter * frequency;

	void setupParametersInternal(int multiplexIndex, bool rangeOnly) override;
	Parameter* setupSingleParameterInternal(Parameter* source, int multiplexIndex, bool rangeOnly) override;
	ProcessResult processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex) override;

	void filterParamChanged(Parameter * p) override;

	String getTypeString() const override { return "FPS"; }


};