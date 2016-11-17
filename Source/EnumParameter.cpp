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


EnumParameterUI * EnumParameter::createUI(EnumParameter * target)
{
	if (target == nullptr) target = this;
	return new EnumParameterUI(target);
}

ControllableUI * EnumParameter::createDefaultUI(Controllable * targetControllable) {
	return createUI(dynamic_cast<EnumParameter *>(targetControllable));
}
