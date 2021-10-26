/*
  ==============================================================================

    MappingLayerPanel.h
    Created: 20 Nov 2016 3:07:42pm
    Author:  Ben Kuper

  ==============================================================================
*/

#pragma once

class MappingLayerPanel :
	public SequenceLayerPanel
{
public:
	MappingLayerPanel(MappingLayer * layer);
	~MappingLayerPanel();

	MappingLayer * mappingLayer;
	std::unique_ptr<ControllableEditor> mappingOutputUI;


	virtual void resizedInternalHeader(Rectangle<int>& r) override;
	void resizedInternalContent(Rectangle<int> &r) override;
	virtual void resizedInternalPanelContent(Rectangle<int>& r) {};

	virtual void addContextMenuItems(PopupMenu& p) override;
	virtual void handleContextMenuResult(int result) override;

};