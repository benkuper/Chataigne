/*
  ==============================================================================

    PJLinkModuleUI.h
    Created: 1 Nov 2022 3:16:55pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class PJLinkClientParamContainerEditor :
    public EnablingControllableContainerEditor
{
public:
    PJLinkClientParamContainerEditor(PJLinkModule::PJLinkClient* c);

    std::unique_ptr<BoolToggleUI> connectedUI;

    void resizedInternalHeader(Rectangle<int>& r) override;
};