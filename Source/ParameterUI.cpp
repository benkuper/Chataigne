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
