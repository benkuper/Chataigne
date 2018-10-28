/*
  ==============================================================================

    ScriptCondition.cpp
    Created: 21 Feb 2017 11:28:06am
    Author:  Ben

  ==============================================================================
*/

#include "ScriptCondition.h"

ScriptCondition::ScriptCondition(var params) :
	Condition(ScriptCondition::getTypeStringStatic(), params),
	script(this,false)
{
	scriptObject.setMethod("setValid", ScriptCondition::setValidFromScript);
	addChildControllableContainer(&script);
	script.userCanRemove = false;
	script.scriptTemplate = "condition";
}

ScriptCondition::~ScriptCondition()
{

}

var ScriptCondition::setValidFromScript(const var::NativeFunctionArgs & a)
{

	ScriptCondition * s = getObjectFromJS<ScriptCondition>(a);
	if (!s->enabled->boolValue()) return var();

	s->isValid->setValue((bool)a.arguments[0]);
	return var();
}

var ScriptCondition::getJSONData()
{
	var data = Condition::getJSONData();
	data.getDynamicObject()->setProperty("script",script.getJSONData());
	return data;
}

void ScriptCondition::loadJSONDataInternal(var data)
{
	Condition::loadJSONDataInternal(data);
	script.loadJSONData(data.getProperty("script", var()));
}
