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
