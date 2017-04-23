/*
  ==============================================================================

    SequenceLayerPanel.cpp
    Created: 17 Nov 2016 7:59:08pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "SequenceLayerPanel.h"

SequenceLayerPanel::SequenceLayerPanel(SequenceLayer * layer) :
	BaseItemUI<SequenceLayer>(layer)
{
	colorUI = item->color->createColorParamUI();

	addAndMakeVisible(colorUI);


	setSize(100, item->uiHeight->intValue());
}

SequenceLayerPanel::~SequenceLayerPanel()
{

}

void SequenceLayerPanel::paintOverChildren(Graphics & g)
{
	BaseItemUI::paintOverChildren(g);
	g.setColour(item->color->getColor());
	g.drawRoundedRectangle(getLocalBounds().reduced(1).toFloat(), 2, 2);
}

void SequenceLayerPanel::resized()
{
	BaseItemUI::resized();
	item->uiHeight->setValue(getHeight());
}

void SequenceLayerPanel::resizedInternalHeader(Rectangle<int>& r)
{
	colorUI->setBounds(r.removeFromRight(r.getHeight()).reduced(2));
}

void SequenceLayerPanel::controllableFeedbackUpdateInternal(Controllable * c)
{
	if (c == item->uiHeight)
	{
		setSize(getWidth(), item->uiHeight->intValue());
	}
	else if (c == item->color)
	{
		repaint();
	}

}
