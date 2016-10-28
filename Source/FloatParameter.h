/*
 ==============================================================================

 FloatParameter.h
 Created: 8 Mar 2016 1:22:10pm
 Author:  bkupe

 ==============================================================================
 */

#ifndef FLOATPARAMETER_H_INCLUDED
#define FLOATPARAMETER_H_INCLUDED



#include "Parameter.h"

class FloatSliderUI;
class FloatStepperUI;

class FloatParameter : public Parameter
{
public:
    FloatParameter(const String &niceName, const String &description, const float &initialValue, const float &minValue = 0.f, const float &maxValue = 1.f, bool enabled = true);
    ~FloatParameter() {}

    void setValueInternal(var & _value) override;

    FloatSliderUI * createSlider(FloatParameter * target = nullptr);
    FloatStepperUI * createStepper(FloatParameter * target = nullptr);
    ControllableUI * createDefaultUI(Controllable * targetControllable = nullptr) override;

   JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FloatParameter)
};


#endif  // FLOATPARAMETER_H_INCLUDED
