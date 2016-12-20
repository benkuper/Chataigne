/*
  ==============================================================================

    FloatStepperUI.cpp
    Created: 28 Apr 2016 6:00:28pm
    Author:  bkupe

  ==============================================================================
*/

#include "FloatStepperUI.h"

FloatStepperUI::FloatStepperUI(Parameter * _parameter) :
    ParameterUI(_parameter)

{
	slider = new BetterStepper(tooltip);
	
    addAndMakeVisible(slider);
    slider->setRange((int)parameter->minimumValue-1, (int)parameter->maximumValue+1,1);
    slider->setValue(parameter->floatValue());
    slider->addListener(this);
	slider->setTextBoxIsEditable(_parameter->isEditable);
}

FloatStepperUI::~FloatStepperUI()
{

}

void FloatStepperUI::resized()
{
    slider->setBounds(getLocalBounds());
}

void FloatStepperUI::valueChanged(const var & value)
{
    if ((float)value == slider->getValue()) return;

    slider->setValue(value,NotificationType::dontSendNotification);
}

void FloatStepperUI::sliderValueChanged(Slider * _slider)
{
	parameter->setValue(_slider->getValue());
}
void FloatStepperUI::rangeChanged(Parameter *){
	slider->setRange((int)parameter->minimumValue - 1, (int)parameter->maximumValue + 1, 1);

}

void FloatStepperUI::setForceFeedbackOnlyInternal()
{
	slider->setTextBoxIsEditable(parameter->isEditable && !forceFeedbackOnly);
}
