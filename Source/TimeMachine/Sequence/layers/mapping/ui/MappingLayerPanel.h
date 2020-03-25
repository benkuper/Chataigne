/*
  ==============================================================================

    MappingLayerPanel.h
    Created: 20 Nov 2016 3:07:42pm
    Author:  Ben Kuper

  ==============================================================================
*/

#pragma once

#include "../MappingLayer.h"
#include "Common/Processor/Mapping/Mapping.h"

class MappingLayerPanel :
	public SequenceLayerPanel
{
public:
	MappingLayerPanel(MappingLayer * layer);
	~MappingLayerPanel();

	MappingLayer * mappingLayer;

	std::unique_ptr<ParameterUI> mappingOutputUI;

	void resizedInternalContent(Rectangle<int> &r) override;
	virtual void resizedInternalPanelContent(Rectangle<int>& r) {};

	void mouseDown(const MouseEvent& e) override;
};