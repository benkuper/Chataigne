/*
  ==============================================================================

    ParameterUI.cpp
    Created: 8 Mar 2016 3:48:44pm
    Author:  bkupe

  ==============================================================================
*/

#include "ParameterUI.h"

//==============================================================================
ParameterUI::ParameterUI(Parameter * parameter) :
parameter(parameter),
ControllableUI(parameter),
showValue(true)
{
    parameter->addAsyncCoalescedListener(this);
    parameter->addParameterListener(this);

}

ParameterUI::~ParameterUI()
{
    if(parameter.get()){
        parameter->removeParameterListener(this);
        parameter->removeAsyncParameterListener(this);
    }
}

void ParameterUI::showEditWindow()
{
	AlertWindow nameWindow("Set a value", "Set a new value for this parameter", AlertWindow::AlertIconType::NoIcon, this);
	nameWindow.addTextEditor("val", parameter->stringValue(), "Value");

	if (parameter->isCustomizableByUser )
	{
		if (parameter->type == Parameter::FLOAT || parameter->type == Parameter::INT)
		{
			nameWindow.addTextEditor("minVal", parameter->minimumValue.toString(), "Minimum");
			nameWindow.addTextEditor("maxVal", parameter->maximumValue.toString(), "Maximum");
		}
	}

	nameWindow.addButton("OK", 1, KeyPress(KeyPress::returnKey));
	nameWindow.addButton("Cancel", 0, KeyPress(KeyPress::escapeKey));

	int result = nameWindow.runModalLoop();

	if (result)
	{
		if (parameter->isCustomizableByUser)
		{
			if (parameter->type == Parameter::FLOAT || parameter->type == Parameter::INT)
			{
				float newMin = nameWindow.getTextEditorContents("minVal").getFloatValue();
				float newMax = nameWindow.getTextEditorContents("maxVal").getFloatValue();
				parameter->setRange(newMin, newMax);
			}
		}

		float newValue = nameWindow.getTextEditorContents("val").getFloatValue();
		parameter->setValue(newValue);
	}
}



bool ParameterUI::shouldBailOut(){
    bool bailOut= parameter.get()==nullptr;
    // we want a clean deletion no?
    jassert(!bailOut);
    return bailOut;

}

// see Parameter::AsyncListener

void ParameterUI::newMessage(const Parameter::ParamWithValue & p) {
	if (p.isRange()) {
		rangeChanged(p.parameter);
	} else {
		valueChanged(p.value);
	}
}
