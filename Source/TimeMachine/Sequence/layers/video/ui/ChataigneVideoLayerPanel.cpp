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
	alphaUI.reset(chataigneVideoLayer->alpha->createSlider());
	addAndMakeVisible(alphaUI.get());
	contentComponents.add(alphaUI.get());
	ndiOutputLabel.setText("NDI", dontSendNotification);
	ndiOutputLabel.setJustificationType(Justification::centredRight);
	ndiOutputLabel.setInterceptsMouseClicks(false, false);
	addAndMakeVisible(ndiOutputLabel);
	contentComponents.add(&ndiOutputLabel);

	ndiOutputUI.reset(chataigneVideoLayer->ndiOutput->createTargetUI());
	addAndMakeVisible(ndiOutputUI.get());
	contentComponents.add(ndiOutputUI.get());
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

	if (!item->miniMode->boolValue())
	{
		alphaUI->setBounds(r.removeFromTop(16).reduced(2));
		Rectangle<int> selector = r.removeFromTop(22);
		ndiOutputLabel.setBounds(selector.removeFromLeft(42));
		ndiOutputUI->setBounds(selector.reduced(2));
	}
}
