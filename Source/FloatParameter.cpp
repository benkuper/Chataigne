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
	argumentsDescription = "float";
}

void FloatParameter::setValueInternal(var & _value)
{
	if ((float)_value < (float)minimumValue && autoAdaptRange) setRange(_value, maximumValue, false);
	else if ((float)_value > (float)maximumValue && autoAdaptRange) setRange(minimumValue, _value, false);
	
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

bool FloatParameter::checkValueIsTheSame(var oldValue, var newValue)
{
	return jlimit<float>(minimumValue, maximumValue, newValue) == (float)oldValue;
}

