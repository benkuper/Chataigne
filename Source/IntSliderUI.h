/*
  ==============================================================================

    IntSliderUI.h
    Created: 8 Mar 2016 3:46:34pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef INTSLIDERUI_H_INCLUDED
#define INTSLIDERUI_H_INCLUDED

#include "FloatSliderUI.h"

class IntSliderUI : public FloatSliderUI
{
public:
    IntSliderUI(Parameter * parameter);
    virtual ~IntSliderUI();

    void setParamNormalizedValue(float value) override;
    float getParamNormalizedValue() override;

protected:
    void valueChanged(const var&) override { repaint(); };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IntSliderUI)
};



#endif  // INTSLIDERUI_H_INCLUDED
