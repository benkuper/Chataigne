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

void AutomationKeyUI::showKeyEditorWindow()
{
	AlertWindow keyEditorWindow("Set key position and value", "Fine tune this key's position and value", AlertWindow::AlertIconType::NoIcon, this);
	keyEditorWindow.addTextEditor("pos", item->position->stringValue(), "Position");
	keyEditorWindow.addTextEditor("val", item->value->stringValue(), "Value");
	

	keyEditorWindow.addButton("OK", 1, KeyPress(KeyPress::returnKey));
	keyEditorWindow.addButton("Cancel", 0, KeyPress(KeyPress::escapeKey));

	int result = keyEditorWindow.runModalLoop();

	if (result)
	{
		float newPos = keyEditorWindow.getTextEditorContents("pos").getFloatValue();
		float newValue = keyEditorWindow.getTextEditorContents("val").getFloatValue();
		item->position->setValue(newPos);
		item->value->setValue(newValue);
	}
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
		if (e.mods.isRightButtonDown())
		{
			PopupMenu p;
			PopupMenu ep;
			StringArray keys = item->easingType->getAllKeys();
			int kid = 1;
			for (auto &i : keys)
			{
				ep.addItem(kid, i, true, i == item->easingType->getValueKey());
				kid++;
			}
			p.addSubMenu("Easing", ep);

			int result = p.show();
			if (result >= 1 && result <= keys.size())
			{
				item->easingType->setValueWithKey(keys[result - 1]);
			}
		}else if (e.mods.isCtrlDown())
		{
			item->easingType->setNext();
		}
	}

	
}

void AutomationKeyUI::mouseDoubleClick(const MouseEvent & e)
{
	showKeyEditorWindow();
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
	handle.color = item->isSelected ? HIGHLIGHT_COLOR : item->isPreselected?PRESELECT_COLOR:FRONT_COLOR;
}

void AutomationKeyUI::inspectablePreselectionChanged(Inspectable * i)
{
	BaseItemMinimalUI::inspectablePreselectionChanged(i);
	handle.highlight = false;
	handle.color = item->isPreselected ? PRESELECT_COLOR : FRONT_COLOR;
}

AutomationKeyUI::Handle::Handle() : 
highlight(false),
color(FRONT_COLOR)
{
	setRepaintsOnMouseActivity(true);
}

void AutomationKeyUI::Handle::paint(Graphics & g)
{
	
	int rad = AutomationKeyUI::handleSize;
	if (isMouseOver() || highlight) rad += 4;
	
	Rectangle<float> er = getLocalBounds().withSizeKeepingCentre(rad,rad).toFloat();
	
	Colour cc = isMouseOver() ? color.brighter() : color.darker(.3f);
	g.setColour(color);
	g.fillEllipse(er);
	g.setColour(cc);
	g.drawEllipse(er, 1);
}

