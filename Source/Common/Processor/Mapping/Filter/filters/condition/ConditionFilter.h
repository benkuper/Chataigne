/*
  ==============================================================================

    ConditionFilter.h
    Created: 16 Feb 2021 12:25:58pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ConditionFilter :
	public MappingFilter
{
public:
	ConditionFilter(var params, Multiplex* multiplex);
	~ConditionFilter();

	ConditionManager cdm;

	void setupParametersInternal(int multiplexIndex) override;
	ProcessResult processInternal(Array<Parameter *> inputs, int multiplexIndex) override;
	ProcessResult processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex) override;

	void updateConditionsLinks(Array<Parameter*> inputs, int multiplexIndex, bool updateLinkNames);

	var getJSONData() override;
	void loadJSONDataItemInternal(var data) override;

	String getTypeString() const override { return "Condition"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConditionFilter)
};