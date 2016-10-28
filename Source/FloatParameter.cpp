/*
  ==============================================================================

    FloatParameter.cpp
    Created: 8 Mar 2016 1:22:10pm
    Author:  bkupe

  ==============================================================================
*/

#include "FloatParameter.h"
#include "FloatSliderUI.h"
#include "FloatStepperUI.h"


FloatParameter::FloatParameter(const String & niceName, const String &description, const float & initialValue, const float & minValue, const float & maxValue, bool enabled) :
    Parameter(Type::FLOAT, niceName, description, (float)initialValue, (float)minValue, (float)maxValue, enabled)
{

}

void FloatParameter::setValueInternal(var & _value)
{
    value = jlimit<double>(minimumValue, maximumValue, _value);
}

FloatSliderUI * FloatParameter::createSlider(FloatParameter * target)
{
	if (target == nullptr) target = this;
    return new FloatSliderUI(target);
}

FloatStepperUI * FloatParameter::createStepper(FloatParameter * target)
{
	if (target == nullptr) target = this;
	return new FloatStepperUI(target);
}

ControllableUI * FloatParameter::createDefaultUI(Controllable * targetControllable) {
    return createSlider(dynamic_cast<FloatParameter *>(targetControllable));
}
