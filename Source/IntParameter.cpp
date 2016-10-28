/*
  ==============================================================================

    IntParameter.cpp
    Created: 8 Mar 2016 1:22:23pm
    Author:  bkupe

  ==============================================================================
*/

#include "IntParameter.h"
#include "IntSliderUI.h"
#include "IntStepperUI.h"

IntParameter::IntParameter(const String & niceName, const String &description, const int & initialValue, const int & minValue, const int & maxValue, bool enabled) :
    Parameter(Type::INT, niceName, description, initialValue, minValue, maxValue, enabled)
{

}

void IntParameter::setValueInternal(var & _value)
{
    this->value = jlimit<int>(minimumValue, maximumValue, _value);
}

IntSliderUI * IntParameter::createSlider(IntParameter * target)
{
	if (target == nullptr) target = this;
	return new IntSliderUI(target);
}

IntStepperUI * IntParameter::createStepper(IntParameter * target)
{
	if (target == nullptr) target = this;
	return new IntStepperUI(target);
}

ControllableUI * IntParameter::createDefaultUI(Controllable * targetControllable){
    return createStepper(dynamic_cast<IntParameter *>(targetControllable));
};
