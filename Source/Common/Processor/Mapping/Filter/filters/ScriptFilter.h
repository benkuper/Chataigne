/*
  ==============================================================================

    ScriptFilter.h
    Created: 21 Feb 2017 6:59:12pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

class ScriptFilter :
	public MappingFilter
{
public:
	ScriptFilter(var params, Multiplex* multiplex);
	~ScriptFilter();

	static String scriptTemplate;
	Script script;

	void onContainerParameterChangedInternal(Parameter* p) override;

	ProcessResult processInternal(Array<Parameter *> inputs, int multiplexIndex) override;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	virtual String getTypeString() const override { return "Script"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScriptFilter)
};
