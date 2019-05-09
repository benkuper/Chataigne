/*
  ==============================================================================

	TimeCueUI.cpp
	Created: 6 Apr 2017 11:13:51am
	Author:  Ben

  ==============================================================================
*/

#include "TimeCueUI.h"
#include "UI/ChataigneAssetManager.h"

TimeCueUI::TimeCueUI(TimeCue * timeCue) :
	BaseItemMinimalUI(timeCue),
	timeAtMouseDown(timeCue->time->floatValue()),
	itemLabel("Label",timeCue->niceName)
{
	dragAndDropEnabled = false;

	bgColor = bgColor.brighter();
	setRepaintsOnMouseActivity(true);
	autoDrawContourWhenSelected = false;
	setSize(10, 20);

	itemLabel.setColour(itemLabel.backgroundColourId, Colours::transparentWhite);
	itemLabel.setColour(itemLabel.textColourId, TEXT_COLOR);

	itemLabel.setColour(itemLabel.backgroundWhenEditingColourId, Colours::black);
	itemLabel.setColour(itemLabel.textWhenEditingColourId, Colours::white);
	itemLabel.setColour(CaretComponent::caretColourId, Colours::orange);
	itemLabel.setFont(11);
	itemLabel.setJustificationType(Justification::centredLeft);

	itemLabel.setEditable(false, item->nameCanBeChangedByUser);
	itemLabel.addListener(this);
	addAndMakeVisible(&itemLabel);

	setTooltip(item->niceName);

	removeMouseListener(this);

	setSize(arrowSize + 12 + itemLabel.getFont().getStringWidth(itemLabel.getText()), getHeight());
}

TimeCueUI::~TimeCueUI()
{
}

void TimeCueUI::paint(Graphics & g)
{
	
	Colour c = item->isSelected ? HIGHLIGHT_COLOR : bgColor;
	if (isMouseOver()) c = c.brighter();
	g.setColour(c);
	g.fillPath(drawPath);
	
	if (item->isLocked->boolValue())
	{
		g.setTiledImageFill(ChataigneAssetManager::getInstance()->smallStripeImage, 0, 0, .1f);
		g.fillPath(drawPath);
	}

	g.setColour(c.darker());
	g.strokePath(drawPath, PathStrokeType(1));

}

void TimeCueUI::resized()
{
	Rectangle<int> r = getLocalBounds();
	Rectangle<int> cr = r.removeFromLeft(arrowSize);
	drawPath.clear();
	drawPath.startNewSubPath(cr.getWidth() / 2.f, (float)cr.getHeight());
	drawPath.lineTo(0, (float)cr.getHeight() - 5);
	drawPath.lineTo(0, 0);
	drawPath.lineTo((float)cr.getWidth(), 0);
	drawPath.lineTo((float)cr.getWidth(), (float)cr.getHeight() - 5);
	drawPath.closeSubPath();
	drawPath = drawPath.createPathWithRoundedCorners(1);


	r.removeFromLeft(1);
	itemLabel.setBounds(r);

}

void TimeCueUI::mouseDoubleClick(const MouseEvent & e)
{
	BaseItemMinimalUI::mouseDoubleClick(e);
	if (e.mods.isCommandDown()) item->remove();
}

void TimeCueUI::mouseDown(const MouseEvent & e)
{
	BaseItemMinimalUI::mouseDown(e);
	timeAtMouseDown = item->time->floatValue();
}

void TimeCueUI::mouseDrag(const MouseEvent & e)
{
	BaseItemMinimalUI::mouseDrag(e);
	if(!item->isLocked->boolValue() && e.eventComponent != &itemLabel) cueUIListeners.call(&TimeCueUIListener::cueDragged, this, e);
}

void TimeCueUI::mouseUp(const MouseEvent & e)
{
	BaseItemMinimalUI::mouseUp(e);
	DBG(item->time->floatValue() << " < > " << timeAtMouseDown);
	if(item->time->floatValue() != timeAtMouseDown) item->time->setUndoableValue(timeAtMouseDown, item->time->floatValue());
}

void TimeCueUI::labelTextChanged(Label * l)
{
	if (l == &itemLabel)
	{
		if (l->getText().isEmpty()) itemLabel.setText(this->baseItem->niceName, dontSendNotification); //avoid setting empty names
		else this->baseItem->setUndoableNiceName(l->getText());
		setSize(arrowSize + 12 + itemLabel.getFont().getStringWidth(itemLabel.getText()), getHeight());
	}
}

void TimeCueUI::controllableFeedbackUpdateInternal(Controllable * c)
{
	if (c == item->time)
	{
		cueUIListeners.call(&TimeCueUIListener::cueTimeChanged, this);
	}
}

void TimeCueUI::containerChildAddressChangedAsync(ControllableContainer * cc)
{
	itemLabel.setText(item->niceName, dontSendNotification);
	setSize(arrowSize + 12 + itemLabel.getFont().getStringWidth(itemLabel.getText()), getHeight());
}
