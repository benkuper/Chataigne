/*
  ==============================================================================

    IntSliderUI.cpp
    Created: 8 Mar 2016 3:46:34pm
    Author:  bkupe

  ==============================================================================
*/

#include "IntSliderUI.h"

IntSliderUI::IntSliderUI(Parameter * parameter) :
    FloatSliderUI(parameter)
{

}

IntSliderUI::~IntSliderUI()
{
}

void IntSliderUI::setParamNormalizedValue(float value)
{
    parameter->setNormalizedValue(value);
}

float IntSliderUI::getParamNormalizedValue()
{
    return parameter->getNormalizedValue();
}
