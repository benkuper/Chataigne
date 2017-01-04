/*
  ==============================================================================

    ShapeShifterPanelTab.cpp
    Created: 3 May 2016 12:43:01pm
    Author:  bkupe

  ==============================================================================
*/

#include "ShapeShifterPanelTab.h"
#include "Style.h"
#include "ShapeShifterManager.h"
#include "AssetManager.h"

ShapeShifterPanelTab::ShapeShifterPanelTab(ShapeShifterContent * _content) : content(_content), selected(false)
{
	panelLabel.setInterceptsMouseClicks(false, false);

	panelLabel.setFont(12);
	panelLabel.setColour(panelLabel.textColourId, TEXT_COLOR);
	panelLabel.setJustificationType(Justification::centred);
	panelLabel.setText(content == nullptr?"[No content]":content->contentName, NotificationType::dontSendNotification);

	addAndMakeVisible(&panelLabel);

	closePanelBT = AssetManager::getInstance()->getRemoveBT();
	closePanelBT->addListener(this);
	addAndMakeVisible(closePanelBT);

	setSize(getLabelWidth(), 20);
}

ShapeShifterPanelTab::~ShapeShifterPanelTab()
{

}

void ShapeShifterPanelTab::setSelected(bool value)
{
	selected = value;
	repaint();
}

void ShapeShifterPanelTab::paint(Graphics & g)
{
	g.setColour(selected?BG_COLOR:BG_COLOR.brighter(.15f));
	Rectangle<int> r = getLocalBounds();
	if (!selected) r.reduce(1,1);
	g.fillRect(r);
}

void ShapeShifterPanelTab::resized()
{
	Rectangle<int> r = getLocalBounds();
	closePanelBT->setBounds(r.removeFromRight(r.getHeight()).reduced(3));
	panelLabel.setBounds(r);
}

int ShapeShifterPanelTab::getLabelWidth()
{
	return panelLabel.getFont().getStringWidth(panelLabel.getText()) + 30;
}

void ShapeShifterPanelTab::buttonClicked(Button * b)
{
	if(b == closePanelBT) tabListeners.call(&TabListener::askForRemoveTab, this);
}
