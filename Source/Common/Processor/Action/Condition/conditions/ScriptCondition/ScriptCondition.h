/*
  ==============================================================================

    ScriptCondition.h
    Created: 21 Feb 2017 11:28:06am
    Author:  Ben

  ==============================================================================
*/

#pragma once

class ScriptCondition :
	public Condition
{
public:
	ScriptCondition(var params, Multiplex* processor = nullptr);
	~ScriptCondition();
	
	static String conditionTemplate;
	Script script;

	//Script
	static var setValidFromScript(const var::NativeFunctionArgs &a);

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	DECLARE_TYPE("Script");
};

