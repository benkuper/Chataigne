/*
  ==============================================================================

    ScriptManager.cpp
    Created: 20 Feb 2017 5:00:57pm
    Author:  Ben

  ==============================================================================
*/

#include "ScriptManager.h"

ScriptManager::ScriptManager(ScriptTarget * _parentTarget):
	BaseManager("Scripts"),
	parentTarget(_parentTarget)
{
	selectItemWhenCreated = false;
}

ScriptManager::~ScriptManager()
{

}

Script * ScriptManager::createItem()
{
	return new Script(parentTarget);
}

bool ScriptManager::callFunctionOnAllItems(const Identifier & function, var a)
{
	Array<var> args;
	args.add(a);
	return callFunctionOnAllItems(function, args);
}

bool ScriptManager::callFunctionOnAllItems(const Identifier & function, Array<var> args)
{
	bool result = true;
	for (auto &i : items) result &= i->callFunction(function, args);
	return result;
}
