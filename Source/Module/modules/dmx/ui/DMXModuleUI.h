/*
  ==============================================================================

    DMXModuleUI.h
    Created: 19 Jun 2020 11:47:57pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../DMXModule.h"

class DMXValueParameterUI :
    public IntStepperUI
{
public:
    DMXValueParameterUI(DMXValueParameter * dmxP);
    ~DMXValueParameterUI();

    DMXValueParameter* dmxP;
    void updateUIParamsInternal() override;

    void addPopupMenuItemsInternal(PopupMenu* p) override;
    void handleMenuSelectedID(int result) override;
};