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
	scriptObject.setMethod("atan2", ScriptUtil::atan2FromScript);
	scriptObject.setMethod("toDegrees", ScriptUtil::toDegrees);
	scriptObject.setMethod("toRadians", ScriptUtil::toRadians);

}

var ScriptUtil::getTime(const var::NativeFunctionArgs &)
{
	return (float)Time::getMillisecondCounter() / 1000.;
}

var ScriptUtil::getFloatFromBytes(const var::NativeFunctionArgs & a)
{
	if (a.numArguments < 4) return 0;
	uint8_t bytes[4];
	for(int i=0;i<4;i++) bytes[i] = (uint8_t)(int)a.arguments[i];
	float result;
	memcpy(&result, &bytes, 4);
	return result;
}

var ScriptUtil::getInt32FromBytes(const var::NativeFunctionArgs & a)
{
	if (a.numArguments < 4) return 0;
	uint8_t bytes[4];
	for (int i = 0; i<4; i++) bytes[i] = (uint8_t)(int)a.arguments[i];
	int result;
	memcpy(&result, &bytes, 4);
	return result;
}

var ScriptUtil::atan2FromScript(const var::NativeFunctionArgs & a)
{
	if (a.numArguments < 2) return 0;
	return atan2((double)a.arguments[0], (double)a.arguments[1]);
}

var ScriptUtil::toDegrees(const var::NativeFunctionArgs & a)
{
	if (a.numArguments < 1) return 0;
	return radiansToDegrees((double)a.arguments[0]);
}

var ScriptUtil::toRadians(const var::NativeFunctionArgs & a)
{
	if (a.numArguments < 1) return 0;
	return degreesToRadians((double)a.arguments[0]);
}
