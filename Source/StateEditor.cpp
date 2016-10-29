/*
  ==============================================================================

    StateEditor.cpp
    Created: 28 Oct 2016 8:27:57pm
    Author:  bkupe

  ==============================================================================
*/

#include "StateEditor.h"

StateEditor::StateEditor(State * state) :
	BaseItemUI<State>(state)
{
	
	setSize(200,250);
	addAndMakeVisible(&grabber);
}

StateEditor::~StateEditor()
{
}

void StateEditor::mouseDown(const MouseEvent & e)
{
	BaseItemUI<State>::mouseDown(e);

	if (e.mods.isLeftButtonDown() && e.eventComponent == &grabber) posAtMouseDown = item->editorPosition->getPoint();
}

void StateEditor::mouseDrag(const MouseEvent & e)
{
	if (e.mods.isLeftButtonDown() && e.eventComponent == &grabber)
	{
		Point<float> targetPos = posAtMouseDown + e.getOffsetFromDragStart().toFloat() / Point<float>((float)getParentComponent()->getWidth(), (float)getParentComponent()->getHeight());
		item->editorPosition->setPoint(targetPos);
	}
}

void StateEditor::resized()
{
	Rectangle<int> r = getLocalBounds().reduced(2);

	grabber.setBounds(r.removeFromTop(grabberHeight));
	grabber.repaint();
	r.removeFromTop(2);

	Rectangle<int> h = r.removeFromTop(headerHeight);
	r.removeFromTop(headerGap);
	Rectangle<int> m = r;

	enabledBT->setBounds(h.removeFromLeft(h.getHeight()));
	h.removeFromLeft(2);
	removeBT->setBounds(h.removeFromRight(h.getHeight()));
	h.removeFromRight(2);
	nameUI->setBounds(h);
}

void StateEditor::controllableFeedbackUpdateInternal(Controllable * c)
{
	if (c == item->editorPosition)
	{
		DBG(item->editorPosition->getPoint().toString());
		Point<int> r = Point<int>(getParentComponent()->getWidth(),getParentComponent()->getHeight());
		setTopLeftPosition((item->editorPosition->getPoint()*r).toInt());
	}
}

//GRABBER

void StateEditor::Grabber::paint(Graphics & g)
{
	Rectangle<int> r = getLocalBounds();
	g.setColour(BG_COLOR.brighter(.2f));
	int numLines = 3;
	for (int i = 0; i < numLines; i++)
	{
		float th = (i + 1)*(float)getHeight() / ((float)numLines + 1);
		g.drawLine(0, th, (float)getWidth(), th, 2);
	}

}
