/*
  ==============================================================================

    ScriptCondition.h
    Created: 21 Feb 2017 11:28:06am
    Author:  Ben

  ==============================================================================
*/

#ifndef SCRIPTCONDITION_H_INCLUDED
#define SCRIPTCONDITION_H_INCLUDED

#include "Condition.h"
#include "Script.h"

class ScriptCondition :
	public Condition
{
public:
	ScriptCondition();
	~ScriptCondition();
	
	Script script;

	//Script
	static var setValidFromScript(const var::NativeFunctionArgs &a);

	InspectableEditor * getEditor(bool isRoot) override;

	String getTypeString() const override { return "Script"; }
	static ScriptCondition * create() { return new ScriptCondition(); }

};



#endif  // SCRIPTCONDITION_H_INCLUDED
