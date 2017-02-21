/*
  ==============================================================================

    ScriptUtil.h
    Created: 21 Feb 2017 8:44:28am
    Author:  Ben

  ==============================================================================
*/

#ifndef SCRIPTUTIL_H_INCLUDED
#define SCRIPTUTIL_H_INCLUDED

#include "ScriptTarget.h"

class ScriptUtil :
	public ScriptTarget
{
public:
	juce_DeclareSingleton(ScriptUtil, true)

	ScriptUtil();
	~ScriptUtil() {}
	static var getTime(const var::NativeFunctionArgs &a);
};


#endif  // SCRIPTUTIL_H_INCLUDED
