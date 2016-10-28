/*
  ==============================================================================

    StringParameter.cpp
    Created: 9 Mar 2016 12:29:30am
    Author:  bkupe

  ==============================================================================
*/

#include "StringParameter.h"
#include "StringParameterUI.h"


StringParameter::StringParameter(const String & niceName, const String &description, const String & initialValue, bool enabled) :
    Parameter(Type::STRING, niceName, description, initialValue, var(), var(), enabled)
{

}



StringParameterUI * StringParameter::createStringParameterUI(StringParameter * target)
{
	if (target == nullptr) target = this;
    return new StringParameterUI(target);
}

ControllableUI* StringParameter::createDefaultUI(Controllable * targetControllable){

    return createStringParameterUI(dynamic_cast<StringParameter *>(targetControllable));
};


void StringParameter::setValue(var _value,bool silentSet,bool force ){
    
   
    if (!force && value.toString() == _value.toString()) return;
    
    setValueInternal(_value);
    
    
    if(_value != defaultValue) isOverriden = true;
    
    if (!silentSet) notifyValueChanged();
};

void  StringParameter::setValueInternal(var & newVal){
    value = newVal.toString();
};

