/*
  ==============================================================================

    BoolToggleUI.h
    Created: 8 Mar 2016 3:47:01pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef BOOLTOGGLEUI_H_INCLUDED
#define BOOLTOGGLEUI_H_INCLUDED

#include "ParameterUI.h"


class BoolToggleUI : public ParameterUI
{
public:
    BoolToggleUI(Parameter * parameter);
    virtual ~BoolToggleUI();

    bool invertVisuals;

    void paint(Graphics &g) override;
    void mouseDown(const MouseEvent &e) override;
    void mouseUp(const MouseEvent &e) override;

protected:
    void valueChanged(const var & ) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BoolToggleUI)
};


#endif  // BOOLTOGGLEUI_H_INCLUDED
