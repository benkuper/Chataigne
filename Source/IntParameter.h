/*
  ==============================================================================

    IntParameter.h
    Created: 8 Mar 2016 1:22:23pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef INTPARAMETER_H_INCLUDED
#define INTPARAMETER_H_INCLUDED

#include "Parameter.h"
class IntSliderUI;
class IntStepperUI;


class IntParameter : public Parameter
{
public:
    IntParameter(const String &niceName, const String &description, const int &initialValue, const int &minimumValue = 0, const int &maximumValue = 1, bool enabled = true);
    ~IntParameter() {}

    void setValueInternal(var & _value) override;

    IntSliderUI * createSlider(IntParameter * target = nullptr);
    IntStepperUI * createStepper(IntParameter * target = nullptr);
    ControllableUI * createDefaultUI(Controllable * targetControllable = nullptr) override;

	static IntParameter * create() { return new IntParameter("New Int Parameter", "", 0); }
	virtual String getTypeString() const override { return "Integer"; }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IntParameter)
};


#endif  // INTPARAMETER_H_INCLUDED
