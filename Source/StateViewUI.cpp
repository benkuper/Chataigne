/*
  ==============================================================================

    StateViewUI.cpp
    Created: 28 Oct 2016 8:27:57pm
    Author:  bkupe

  ==============================================================================
*/

#include "StateViewUI.h"

StateViewUI::StateViewUI(State * state) :
	BaseItemUI<State>(state),
	amui(&state->am),
	mmui(&state->mm),
	resizer(this,nullptr)
{
	setSize(200,250);
	
	amui.drawContour = true;
	mmui.drawContour = true;
	amui.bgColor = ACTION_COLOR.withSaturation(.2f).darker(1);
	mmui.bgColor = MAPPING_COLOR.withSaturation(.2f).darker(1);

	addAndMakeVisible(&amui);
	addAndMakeVisible(&mmui);

	addAndMakeVisible(&grabber);
	addAndMakeVisible(&resizer);
}

StateViewUI::~StateViewUI()
{
}

void StateViewUI::mouseDown(const MouseEvent & e)
{
	BaseItemUI<State>::mouseDown(e);

	if (e.mods.isLeftButtonDown())
	{
		if (e.eventComponent == &grabber) posAtMouseDown = item->editorPosition->getPoint();
	}
}

void StateViewUI::mouseDrag(const MouseEvent & e)
{
	if (e.mods.isLeftButtonDown() && e.eventComponent == &grabber)
	{
		Point<float> targetPos = posAtMouseDown + e.getOffsetFromDragStart().toFloat() / Point<float>((float)getParentComponent()->getWidth(), (float)getParentComponent()->getHeight());
		item->editorPosition->setPoint(targetPos);
	}
}

void StateViewUI::resized()
{
	Rectangle<int> r = getLocalBounds().reduced(2);

	//Grabber
	grabber.setBounds(r.removeFromTop(grabberHeight));
	grabber.repaint();
	r.removeFromTop(2);

	//Resizer
	resizer.setBounds(r.removeFromBottom(10).withLeft(getWidth() - 10));


	//Header
	Rectangle<int> h = r.removeFromTop(headerHeight);
	r.removeFromTop(headerGap);

	enabledBT->setBounds(h.removeFromLeft(h.getHeight()));
	h.removeFromLeft(2);
	removeBT->setBounds(h.removeFromRight(h.getHeight()));
	h.removeFromRight(2);
	nameUI->setBounds(h);


	//Actions
	amui.setBounds(r.removeFromTop(r.getHeight()/2 - 5));
	r.removeFromTop(5);
	//Mapping
	r.removeFromTop(5);
	mmui.setBounds(r);
}


void StateViewUI::controllableFeedbackUpdateInternal(Controllable * c)
{
	if (c == item->editorPosition) stateEditorListeners.call(&StateViewUI::Listener::editorGrabbed, this);
}

//GRABBER

void StateViewUI::Grabber::paint(Graphics & g)
{
	Rectangle<int> r = getLocalBounds();
	g.setColour(BG_COLOR.brighter(.2f));
	int numLines = 3;
	for (int i = 0; i < numLines; i++)
	{
		float th = (i + 1)*(float)getHeight() / ((float)numLines + 1);
		g.drawLine(0, th, (float)getWidth(), th, 1);
	}
}
