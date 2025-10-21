/*
  ==============================================================================

    MathFilter.h
    Created: 4 Jul 2018 2:15:50pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

class MathFilter :
	public MappingFilter
{
public:
	MathFilter(var params, Multiplex* multiplex);
	~MathFilter();

	enum Operation { OFFSET, MULTIPLY, DIVIDE, MODULO, FLOOR, CEIL, ROUND, MAX, MIN, ABSOLUTE, LOG, EXPONENTIAL };
	enum RangeRemapMode { KEEP, AJDUST, FREE };
	EnumParameter * operation;
	Parameter * operationValue;

	EnumParameter* rangeRemapMode;
	var opValueData; //for loading after setupParamInternal

	void setupParametersInternal(int multiplexIndex, bool rangeOnly) override;
	ProcessResult processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex) override;

	bool updateFilteredParamsRange(int multiplexIndex);
	void filterParamChanged(Parameter * p) override;
	void parameterControlModeChanged(Parameter* p) override;

	float getProcessedValue(float val, int index, int multiplexIndex);


	bool filteredParamShouldHaveRange();

	var getJSONData(bool includeNonOverriden = false) override;
	void loadJSONDataInternal(var data) override;

	virtual String getTypeString() const override { return "Math"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MathFilter)
};