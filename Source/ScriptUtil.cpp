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
	scriptObject.setMethod("getFloatFromBytes", ScriptUtil::getFloatFromBytes);
	scriptObject.setMethod("getInt32FromBytes", ScriptUtil::getInt32FromBytes);

}

var ScriptUtil::getTime(const var::NativeFunctionArgs &)
{
	return (float)Time::getMillisecondCounter() / 1000.;
}

var ScriptUtil::getFloatFromBytes(const var::NativeFunctionArgs & a)
{
	if (a.numArguments < 4) return 0;
	uint8_t bytes[4];
	for(int i=0;i<4;i++) bytes[i] = (int)a.arguments[i];
	float result;
	memcpy(&result, &bytes, 4);
	return result;
}

var ScriptUtil::getInt32FromBytes(const var::NativeFunctionArgs & a)
{
	if (a.numArguments < 4) return 0;
	uint8_t bytes[4];
	for (int i = 0; i<4; i++) bytes[i] = (int)a.arguments[i];
	int result;
	memcpy(&result, &bytes, 4);
	return result;
}
