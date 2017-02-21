/*
  ==============================================================================

    ScriptUtil.cpp
    Created: 21 Feb 2017 9:17:23am
    Author:  Ben

  ==============================================================================
*/

#include "ScriptUtil.h"

juce_ImplementSingleton(ScriptUtil)

ScriptUtil::ScriptUtil() :
	ScriptTarget("util", this)
{
	scriptObject.setMethod("getTime", ScriptUtil::getTime);
}

var ScriptUtil::getTime(const var::NativeFunctionArgs &)
{
	return (float)Time::getMillisecondCounter() / 1000.;
}
