/*
  ==============================================================================

    FreezeFilter.h
    Created: 16 Apr 2022 9:35:18pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class FreezeFilter :
	public TimeFilter
{
public:
	FreezeFilter(var params, Multiplex* multiplex);
	~FreezeFilter();

	enum FreezeMode{ MAX, MIN, HOLD };
	EnumParameter* mode;
	Trigger* reset;
	HashMap<Parameter*, var> prevValueMap;

	void setupParametersInternal(int multiplexIndex, bool rangeOnly) override;
	Parameter* setupSingleParameterInternal(Parameter* source, int multiplexIndex, bool rangeOnly) override;
	virtual ProcessResult processSingleParameterTimeInternal(Parameter* source, Parameter* out, int multiplexIndex, double deltaTime) override;

	void onContainerTriggerTriggered(Trigger* t) override;

	String getTypeString() const override { return "Freeze"; }
};
