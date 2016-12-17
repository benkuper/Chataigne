/*
  ==============================================================================

    AutomationKeyUI.cpp
    Created: 11 Dec 2016 1:22:27pm
    Author:  Ben

  ==============================================================================
*/

#include "AutomationKeyUI.h"


AutomationKeyUI::AutomationKeyUI(AutomationKey * key) :
	BaseItemMinimalUI(key)
{

	addAndMakeVisible(&handle);

	//removeMouseListener(this);

	autoDrawHighlightWhenSelected = false;
	setRepaintsOnMouseActivity(true);

	setEasingUI(item->easing != nullptr ? item->easing->createUI() : nullptr);
}

AutomationKeyUI::~AutomationKeyUI()
{
}


void AutomationKeyUI::setEasingUI(EasingUI * eui)
{
	if (easingUI != nullptr)
	{
		removeChildComponent(easingUI);
	}
	easingUI = eui;

	if (easingUI != nullptr)
	{
		
		addAndMakeVisible(easingUI);
		easingUI->toBack();
		resized();
		easingUI->setKeyPositions(keyYPos1, keyYPos2);
	}
	
}

void AutomationKeyUI::setKeyPositions(const int &k1, const int &k2)
{
	keyYPos1 = k1;
	keyYPos2 = k2;
	if (easingUI != nullptr) easingUI->setKeyPositions(keyYPos1, keyYPos2);

	Rectangle<int> hr = getLocalBounds().withSize(AutomationKeyUI::handleClickZone, AutomationKeyUI::handleClickZone)
		.withCentre(Point<int>(AutomationKeyUI::handleClickZone / 2, (int)((1 - item->value->floatValue())*getHeight())));

	handle.setBounds(hr);
}

void AutomationKeyUI::resized()
{

	Rectangle<int> hr = getLocalBounds().withSize(AutomationKeyUI::handleClickZone, AutomationKeyUI::handleClickZone)
		.withCentre(Point<int>(AutomationKeyUI::handleClickZone / 2, (int)((1 - item->value->floatValue())*getHeight())));

	handle.setBounds(hr);

	Rectangle<int> r = getLocalBounds().reduced(AutomationKeyUI::handleClickZone / 2,0);
	if (easingUI != nullptr)
	{
		easingUI->setBounds(r);
	}
}

bool AutomationKeyUI::hitTest(int tx, int ty)
{
	return handle.getBounds().contains(tx, ty) || (easingUI != nullptr && easingUI->hitTest(tx,ty));
}

void AutomationKeyUI::mouseDown(const MouseEvent & e)
{
	BaseItemMinimalUI::mouseDown(e);
	if (e.eventComponent == &handle)
	{
		if (e.mods.isLeftButtonDown())
		{
			if (e.mods.isCtrlDown()) item->remove();
		}
	}
	else if (e.eventComponent == easingUI)
	{
		if (e.mods.isCtrlDown())
		{
			item->easingType->setNext();
		}
	}

	
}

void AutomationKeyUI::controllableFeedbackUpdateInternal(Controllable * c)
{
	if(c == item->easingType)
	{
		setEasingUI(item->easing != nullptr?item->easing->createUI():nullptr);
	}
}

void AutomationKeyUI::inspectableSelectionChanged(Inspectable * i)
{
	BaseItemMinimalUI::inspectableSelectionChanged(i);
	handle.highlight = item->isSelected;
}

AutomationKeyUI::Handle::Handle() : 
	highlight(false)
{
	setRepaintsOnMouseActivity(true);
}

void AutomationKeyUI::Handle::paint(Graphics & g)
{
	
	int rad = AutomationKeyUI::handleSize;
	if (isMouseOver() || highlight) rad += 4;
	
	Rectangle<float> er = getLocalBounds().withSizeKeepingCentre(rad,rad).toFloat();
	
	Colour c = highlight ? HIGHLIGHT_COLOR : FRONT_COLOR;
	Colour cc = isMouseOver() ? c.brighter() : c.darker(.3f);
	g.setColour(c);
	g.fillEllipse(er);
	g.setColour(cc);
	g.drawEllipse(er, 1);
}

