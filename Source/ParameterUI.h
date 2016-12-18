/*
  ==============================================================================

    ParameterUI.h
    Created: 8 Mar 2016 3:48:44pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef PARAMETERUI_H_INCLUDED
#define PARAMETERUI_H_INCLUDED

#include "Parameter.h"
#include "ControllableUI.h"

class ParameterUI : public Parameter::AsyncListener, public Parameter::Listener,public ControllableUI
{
public:
    ParameterUI(Parameter * parameter);
    virtual ~ParameterUI();

    WeakReference<Parameter> parameter;

	bool showValue;

protected:


    // helper to spot wrong deletion order
    bool shouldBailOut();

    // here we are bound to only one parameter so no need to pass parameter*
    // for general behaviour see AsyncListener
    virtual void valueChanged(const var & ){};
	virtual void rangeChanged(Parameter * ){};


protected:
    // see Parameter::AsyncListener
	virtual void newMessage(const Parameter::ParamWithValue & p) override;;

    // never change this as value can be changed from other threads
    void parameterValueChanged(Parameter * ) override{};
    void parameterRangeChanged(Parameter * ) override{};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParameterUI)
};


#endif  // PARAMETERUI_H_INCLUDED
