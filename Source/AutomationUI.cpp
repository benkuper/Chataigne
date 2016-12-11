/*
  ==============================================================================

    AutomationUI.cpp
    Created: 11 Dec 2016 1:22:02pm
    Author:  Ben

  ==============================================================================
*/

#include "AutomationUI.h"

AutomationUI::AutomationUI(Automation * _automation) :
	BaseManagerUI("Automation",_automation,false)
{
	transparentBG = true;
	useDefaultMenu = false;

}

AutomationUI::~AutomationUI()
{
}

void AutomationUI::paint(Graphics & g)
{
	BaseManagerUI::paint(g);

	if (itemsUI.size() < 2) return;
	int len = itemsUI.size() - 1;
	for (int i = 0; i < len; i++)
	{
		g.setColour(Colours::white);
		Point<int> p1 = itemsUI[i]->getBounds().getCentre();
		Point<int> p2 = itemsUI[i + 1]->getBounds().getCentre();
		g.drawLine(p1.x, p1.y, p2.x, p2.y, 2);
	}
}

void AutomationUI::resized()
{
	const float pointSize = 6;
	for (auto &kui : itemsUI)
	{
		int tx = (kui->item->position->floatValue()/manager->positionMax)*getWidth() - pointSize/2;
		int ty = (1 - kui->item->value->floatValue() / manager->valueMax)*getHeight() - pointSize / 2;
		kui->setBounds(tx, ty, pointSize,pointSize);
	}
}

void AutomationUI::itemsReordered()
{
	BaseManagerUI::itemsReordered();
	repaint();
}

void AutomationUI::mouseDown(const MouseEvent & e)
{
	BaseManagerUI::mouseDown(e);
	if (e.mods.isCtrlDown())
	{
		manager->addItem((e.getPosition().x*manager->positionMax) / getWidth(), (1 - e.getPosition().y*1.f / getHeight())*manager->valueMax);
		resized();
	}
}

