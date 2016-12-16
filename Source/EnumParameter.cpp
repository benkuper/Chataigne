/*
  ==============================================================================

    EnumParameter.cpp
    Created: 29 Sep 2016 5:34:59pm
    Author:  bkupe

  ==============================================================================
*/

#include "EnumParameter.h"
#include "EnumParameterUI.h"


EnumParameter::EnumParameter(const String & niceName, const String &description, bool enabled) :
	Parameter(Type::ENUM, niceName, description,String::empty,var(),var(), enabled)
{

}

void EnumParameter::addOption(String key, var data)
{
	enumValues.set(key, data);
	enumListeners.call(&Listener::enumOptionAdded, this, key);

	updateArgDescription();
}

void EnumParameter::removeOption(String key)
{
	enumValues.remove(key);
	enumListeners.call(&Listener::enumOptionRemoved, this, key);
	updateArgDescription();
}

void EnumParameter::updateArgDescription()
{
	argumentsDescription = "";
	HashMap<String, var>::Iterator i(enumValues);
	int index = 1;
	while (i.next())
	{
		argumentsDescription += i.getKey();
		if(index < enumValues.size()-1) argumentsDescription += " | ";
		index++;
	}

}

void EnumParameter::setValueWithKey(String key)
{
	DBG("set value " << key);
	setValue(key);
}

void EnumParameter::setNext(bool loop)
{
	DBG("set next");
	HashMap<String, var>::Iterator i(enumValues);
	int index = 0;
	String firstKey;
	while (i.next())
	{
		if (index == 0) firstKey = i.getKey(); 
		if (i.getKey() == getValueKey())
		{
			DBG("found current key");
			if (i.next())
			{
				DBG("next is 0 <<" << i.getKey());
				setValueWithKey(i.getKey());
			}
			else if (loop)
			{
				setValueWithKey(firstKey);
				
			}
			return;
		}
		index++;
	}
}

int EnumParameter::getKeyIndex(String key)
{
	HashMap<String, var>::Iterator i(enumValues);
	int index = 0;
	while (i.next())
	{
		if (i.getKey() == key) return index;
		index++;
	}
	return -1;
}

EnumParameterUI * EnumParameter::createUI(EnumParameter * target)
{
	if (target == nullptr) target = this;
	return new EnumParameterUI(target);
}

ControllableUI * EnumParameter::createDefaultUI(Controllable * targetControllable) {
	return createUI(dynamic_cast<EnumParameter *>(targetControllable));
}
