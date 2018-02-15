/*
  ==============================================================================

    ScriptCondition.h
    Created: 21 Feb 2017 11:28:06am
    Author:  Ben

  ==============================================================================
*/

#ifndef SCRIPTCONDITION_H_INCLUDED
#define SCRIPTCONDITION_H_INCLUDED

#include "../../Condition.h"


class ScriptCondition :
	public Condition
{
public:
	ScriptCondition(var params);
	~ScriptCondition();
	
	Script script;

	//Script
	static var setValidFromScript(const var::NativeFunctionArgs &a);

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	String getTypeString() const override { return ScriptCondition::getTypeStringStatic(); }
	static String getTypeStringStatic() { return "Script"; }
	static ScriptCondition * create(var params) { return new ScriptCondition(params); }

};

#endif  // SCRIPTCONDITION_H_INCLUDED
