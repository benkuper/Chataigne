/*
  ==============================================================================

    AutomationMappingLayerPanel.h
    Created: 25 Mar 2020 1:46:10pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class AutomationMappingLayerPanel :
	public MappingLayerPanel
{
public:
	AutomationMappingLayerPanel(AutomationMappingLayer* layer);
	virtual ~AutomationMappingLayerPanel();

    std::unique_ptr<BoolToggleUI> armUI;
	std::unique_ptr<ImageButton> keyBT;
	std::unique_ptr<ImageButton> prevKeyBT;
	std::unique_ptr<ImageButton> nextKeyBT;

	AutomationMappingLayer* aml;
    
    virtual void resizedInternalPanelContent(Rectangle<int>& r) override;

	virtual void buttonClicked(Button* b) override;
};