/*
  ==============================================================================

    FreezeFilter.h
    Created: 16 Apr 2022 9:35:18pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class FreezeFilter :
	public MappingFilter
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
	virtual ProcessResult processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex) override;

	void onContainerTriggerTriggered(Trigger* t) override;

	String getTypeString() const override { return "Freeze"; }
};
