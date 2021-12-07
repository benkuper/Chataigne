/*
  ==============================================================================

    DMXModuleUI.h
    Created: 19 Jun 2020 11:47:57pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class DMXValueParameterUI :
    public IntStepperUI
{
public:
    DMXValueParameterUI(Array<DMXValueParameter *> dmxP);
    ~DMXValueParameterUI();

    Array<DMXValueParameter*> dmxParameters;
    void updateUIParamsInternal() override;

    void addPopupMenuItemsInternal(PopupMenu* p) override;
    void handleMenuSelectedID(int result) override;
};