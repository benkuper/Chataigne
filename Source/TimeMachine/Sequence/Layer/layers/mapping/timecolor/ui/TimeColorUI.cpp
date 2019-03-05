/*
  ==============================================================================

    TimeColorUI.cpp
    Created: 11 Apr 2017 11:40:31am
    Author:  Ben

  ==============================================================================
*/

#include "TimeColorUI.h"

TimeColorUI::TimeColorUI(TimeColor * item) :
	BaseItemMinimalUI(item)
{
	autoDrawContourWhenSelected = false;
	colorUI = item->color->createColorParamUI();
	addAndMakeVisible(colorUI);
	//colorUI->addMouseListener(this, false); //why ??
}

TimeColorUI::~TimeColorUI()
{
	
}

void TimeColorUI::paint(Graphics & g)
{
	g.setColour(item->isSelected ? HIGHLIGHT_COLOR:bgColor);
	g.fillPath(drawPath);
	g.setColour(item->isSelected?HIGHLIGHT_COLOR.brighter():bgColor.darker(.7f));
	g.strokePath(drawPath,PathStrokeType(1));
}

void TimeColorUI::resized()
{
	drawPath.clear();
	drawPath.startNewSubPath((float)getWidth() / 2, 0);
	drawPath.lineTo((float)getWidth(), 4);
	drawPath.lineTo((float)getWidth(), (float)getHeight());
	drawPath.lineTo(0, (float)getHeight());
	drawPath.lineTo(0, 4);
	drawPath.closeSubPath();
	drawPath = drawPath.createPathWithRoundedCorners(1);

	colorUI->setBounds(getLocalBounds().reduced(2, getHeight()/4));
}

void TimeColorUI::mouseDown(const MouseEvent & e)
{
	BaseItemMinimalUI::mouseDown(e);
	posAtMouseDown = item->position->floatValue();
	if (e.mods.isCommandDown())
	{
		item->interpolation->setNext(true, true);
	}
}

void TimeColorUI::mouseUp(const MouseEvent & e)
{
	item->position->setUndoableValue(posAtMouseDown, item->position->floatValue());
}
