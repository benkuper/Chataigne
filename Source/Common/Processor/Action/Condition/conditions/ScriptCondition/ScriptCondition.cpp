/*
  ==============================================================================

    ScriptCondition.cpp
    Created: 21 Feb 2017 11:28:06am
    Author:  Ben

  ==============================================================================
*/

#include "ScriptCondition.h"
#include "UI/ChataigneAssetManager.h"

String ScriptCondition::conditionTemplate = "";

ScriptCondition::ScriptCondition(var params,IteratorProcessor * processor) :
	Condition(ScriptCondition::getTypeStringStatic(), params, processor),
	script(this,false)
{
	scriptObject.setMethod("setValid", ScriptCondition::setValidFromScript);
	addChildControllableContainer(&script);
	script.userCanRemove = false;
	
	if(conditionTemplate.isEmpty()) conditionTemplate = ChataigneAssetManager::getInstance()->getScriptTemplate("condition");
	script.scriptTemplate = &conditionTemplate; 
}

ScriptCondition::~ScriptCondition()
{

}

var ScriptCondition::setValidFromScript(const var::NativeFunctionArgs & a)
{

	ScriptCondition * s = getObjectFromJS<ScriptCondition>(a);
	if (!s->enabled->boolValue()) return var();

	if (!checkNumArgs(s->niceName, a, 1)) return var();

	if (a.numArguments == 1)
	{
		for (int i = 0; i < s->getIterationCount(); i++) s->setValid(i, (bool)a.arguments[0]);
	}
	else
	{
		s->setValid((int)a.arguments[0], (bool)a.arguments[1]);
	}

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
