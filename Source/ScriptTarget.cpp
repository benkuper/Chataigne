/*
  ==============================================================================

    ScriptTarget.cpp
    Created: 20 Feb 2017 7:49:11pm
    Author:  Ben

  ==============================================================================
*/

#include "ScriptTarget.h"

ScriptTarget::ScriptTarget(const String & name, void * ptr) :
	scriptTargetName(name),
	thisPtr((int64)ptr)
{
	
}

ScriptTarget::~ScriptTarget()
{
}



DynamicObject * ScriptTarget::createScriptObject()
{
	DynamicObject * o = new DynamicObject(scriptObject);
	o->setProperty(scriptPtrIdentifier, thisPtr);
	return o;
}