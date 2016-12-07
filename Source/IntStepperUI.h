/*
  ==============================================================================

    IntStepperUI.h
    Created: 8 Mar 2016 3:46:43pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef INTSTEPPERUI_H_INCLUDED
#define INTSTEPPERUI_H_INCLUDED

#include "FloatStepperUI.h"

class IntStepperUI : public FloatStepperUI 
{
public:
    IntStepperUI(Parameter * parameter);
    virtual ~IntStepperUI();



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IntStepperUI)
};


#endif  // INTSTEPPERUI_H_INCLUDED
