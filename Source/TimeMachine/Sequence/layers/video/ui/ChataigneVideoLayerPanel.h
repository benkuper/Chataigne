/*
  ==============================================================================

    ChataigneVideoLayerPanel.h
    Created: 26 Sep 2026

  ==============================================================================
*/

#pragma once

#include "../ChataigneVideoLayer.h"

class ChataigneVideoLayerPanel :
	public VideoLayerPanel
{
public:
	ChataigneVideoLayerPanel(ChataigneVideoLayer* layer);
	~ChataigneVideoLayerPanel();

	ChataigneVideoLayer* chataigneVideoLayer;

	void resizedInternalHeader(Rectangle<int>& r) override;
	void resizedInternalContent(Rectangle<int>& r) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChataigneVideoLayerPanel)
};