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
	if ((float)value < (float)minimumValue && autoAdaptRange) setRange(value, maximumValue, false);
	else if ((float)value > (float)maximumValue && autoAdaptRange) setRange(minimumValue, value, false);
	
	if ((float)minimumValue > (float)maximumValue) return;
	value = jlimit<float>(minimumValue, maximumValue, _value);
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
