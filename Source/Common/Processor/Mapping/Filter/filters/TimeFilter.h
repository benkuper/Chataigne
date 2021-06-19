/*
  ==============================================================================

    TimeFilter.h
    Created: 18 Jun 2021 8:44:36pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class TimeFilter :
	public MappingFilter
{
public:
	TimeFilter(StringRef name, var params, Multiplex* multiplex);
	virtual ~TimeFilter();

	Array<double> timesAtLastUpdate; //multiplexed
	Array<double> deltaTimes; //multiplexed

	virtual void multiplexCountChanged() override;

	ProcessResult processInternal(Array<Parameter*> sources, int multiplexIndex) override;
	ProcessResult processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex) override;
	virtual ProcessResult processSingleParameterTimeInternal(Parameter* source, Parameter* out, int multiplexIndex, double deltaTime) { return ProcessResult::UNCHANGED;  }
};
