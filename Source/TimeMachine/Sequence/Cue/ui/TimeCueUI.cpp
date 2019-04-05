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
	timeAtMouseDown(timeCue->time->floatValue())
{
	dragAndDropEnabled = false;

	bgColor = bgColor.brighter();
	setRepaintsOnMouseActivity(true);
	autoDrawContourWhenSelected = false;
	setSize(10, 20);

	setTooltip(item->niceName);



	removeMouseListener(this);
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
	drawPath.clear();
	drawPath.startNewSubPath(getWidth() / 2.f, (float)getHeight());
	drawPath.lineTo(0, (float)getHeight() - 5);
	drawPath.lineTo(0, 0);
	drawPath.lineTo((float)getWidth(), 0);
	drawPath.lineTo((float)getWidth(), (float)getHeight() - 5);
	drawPath.closeSubPath();
	drawPath = drawPath.createPathWithRoundedCorners(1);
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
	if(!item->isLocked->boolValue()) cueUIListeners.call(&TimeCueUIListener::cueDragged, this, e);
}

void TimeCueUI::mouseUp(const MouseEvent & e)
{
	BaseItemMinimalUI::mouseUp(e);
	DBG(item->time->floatValue() << " < > " << timeAtMouseDown);
	if(item->time->floatValue() != timeAtMouseDown) item->time->setUndoableValue(timeAtMouseDown, item->time->floatValue());
}

void TimeCueUI::controllableFeedbackUpdateInternal(Controllable * c)
{
	if (c == item->time)
	{
		cueUIListeners.call(&TimeCueUIListener::cueTimeChanged, this);
	}
}
