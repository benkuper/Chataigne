/*
  ==============================================================================

    ScriptTarget.h
    Created: 20 Feb 2017 7:49:11pm
    Author:  Ben

  ==============================================================================
*/

#ifndef SCRIPTTARGET_H_INCLUDED
#define SCRIPTTARGET_H_INCLUDED

#include "JuceHeader.h"

const Identifier scriptPtrIdentifier = "_ptr";

class ScriptTarget
{
public:
	ScriptTarget(const String &name, void * ptr) : scriptTargetName(name), thisPtr((int64)ptr) {}
	virtual ~ScriptTarget() {}

	int64 thisPtr;
	String scriptTargetName;
	DynamicObject scriptObject;

	virtual DynamicObject * createScriptObject() {
		DynamicObject * o = new DynamicObject(scriptObject);
		o->setProperty(scriptPtrIdentifier, thisPtr);
		return o;
	}
	 
	template<class T>
	static T* getObjectFromJS(const var::NativeFunctionArgs & a);
};

template<class T>
inline T * ScriptTarget::getObjectFromJS(const var::NativeFunctionArgs & a) {
	DynamicObject * d = a.thisObject.getDynamicObject();
	if (d == nullptr) return nullptr;
	return reinterpret_cast<T*>((T*)(int64)d->getProperty(scriptPtrIdentifier));
}

#endif  // SCRIPTTARGET_H_INCLUDED
