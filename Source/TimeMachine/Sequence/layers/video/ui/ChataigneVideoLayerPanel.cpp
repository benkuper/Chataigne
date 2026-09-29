/*
  ==============================================================================

    ChataigneVideoLayerPanel.cpp
    Created: 26 Sep 2026

  ==============================================================================
*/

#include "ChataigneVideoLayerPanel.h"

ChataigneVideoLayerPanel::ChataigneVideoLayerPanel(ChataigneVideoLayer* layer) :
	VideoLayerPanel(layer),
	chataigneVideoLayer(layer)
{
}

ChataigneVideoLayerPanel::~ChataigneVideoLayerPanel()
{
}

void ChataigneVideoLayerPanel::resizedInternalHeader(Rectangle<int>& r)
{
	VideoLayerPanel::resizedInternalHeader(r);
}

void ChataigneVideoLayerPanel::resizedInternalContent(Rectangle<int>& r)
{
	VideoLayerPanel::resizedInternalContent(r);
}