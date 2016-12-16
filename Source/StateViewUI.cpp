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
	
	amui.drawContour = true;
	mmui.drawContour = true;
	amui.bgColor = ACTION_COLOR.withSaturation(.2f).darker(1);
	mmui.bgColor = MAPPING_COLOR.withSaturation(.2f).darker(1);
	amui.addManagerUIListener(this);
	mmui.addManagerUIListener(this);

	addAndMakeVisible(&grabber);

	contentContainer.addAndMakeVisible(&amui);
	contentContainer.addAndMakeVisible(&mmui);	

	
	setSize(item->viewUISize->getPoint().x,item->viewUISize->getPoint().y);
	updateMiniModeUI();
	
}

StateViewUI::~StateViewUI()
{
}

void StateViewUI::updateMiniModeUI()
{
	if (item->miniMode->boolValue())
	{
		removeChildComponent(&contentContainer);
		removeChildComponent(&resizer);
		setSize(getWidth(), grabberHeight + headerHeight + 10);
	} else
	{
		addAndMakeVisible(&contentContainer);
		addAndMakeVisible(&resizer);
		setSize(getWidth(), contentContainer.getBottom());
	}
	
}

void StateViewUI::mouseDown(const MouseEvent & e)
{
	BaseItemUI<State>::mouseDown(e);

	if (e.mods.isLeftButtonDown())
	{
		if (e.eventComponent == &grabber) posAtMouseDown = item->viewUIPosition->getPoint();
	}
}

void StateViewUI::mouseDrag(const MouseEvent & e)
{
	if (e.mods.isLeftButtonDown() && e.eventComponent == &grabber)
	{
		Point<float> targetPos = posAtMouseDown + e.getOffsetFromDragStart().toFloat() / Point<float>((float)getParentComponent()->getWidth(), (float)getParentComponent()->getHeight());
		item->viewUIPosition->setPoint(targetPos);
	}
}

void StateViewUI::mouseDoubleClick(const MouseEvent & e)
{
	if (e.eventComponent == &grabber) item->miniMode->setValue(!item->miniMode->boolValue());
}

void StateViewUI::resized()
{
	Rectangle<int> r = getLocalBounds().reduced(2);

	//Grabber
	grabber.setBounds(r.removeFromTop(grabberHeight));
	grabber.repaint(); 

	//Resizer
	if (!item->miniMode->boolValue())
	{
		resizer.setBounds(r.removeFromBottom(10).withLeft(getWidth() - 10));
	}

	//Header
	Rectangle<int> h = r.removeFromTop(headerHeight);
	r.removeFromTop(headerGap);

	enabledBT->setBounds(h.removeFromLeft(h.getHeight()));
	h.removeFromLeft(2);
	removeBT->setBounds(h.removeFromRight(h.getHeight()));
	h.removeFromRight(2);
	nameUI->setBounds(h);

	
	if (!item->miniMode->boolValue())
	{
		contentContainer.setBounds(r);
		Rectangle<int> cr = contentContainer.getLocalBounds();
		int gap = 4;
		int amHeight = jmax<int>(amui.getContentHeight(), 30);
		int mmHeight = jmax<int>(mmui.getContentHeight(), 30);

		item->viewUISize->setPoint(getWidth(), getHeight());

		if ((amHeight > cr.getHeight()/2 || mmHeight > cr.getHeight() /2) && amHeight + mmHeight + gap <= cr.getHeight())
		{
			if (amHeight > mmHeight)
			{
				amui.setBounds(cr.removeFromTop(amHeight));
				cr.removeFromTop(gap);
				mmui.setBounds(cr);
			} else
			{
				mmui.setBounds(cr.removeFromBottom(mmHeight));
				cr.removeFromBottom(gap);
				amui.setBounds(cr);
			}
		} else
		{
			amui.setBounds(cr.removeFromTop((cr.getHeight() - gap) / 2));
			cr.removeFromTop(gap);
			mmui.setBounds(cr);
		}
	} 
}

void StateViewUI::childBoundsChanged(Component *)
{
	resized();
}

void StateViewUI::controllableFeedbackUpdateInternal(Controllable * c)
{
	if (c == item->viewUIPosition) stateEditorListeners.call(&StateViewUI::Listener::editorGrabbed, this);
	else if (c == item->miniMode) updateMiniModeUI();
}

void StateViewUI::itemUIAdded(ActionUI *)
{
	resized();
}

void StateViewUI::itemUIRemoved(ActionUI *)
{
	resized();
}

void StateViewUI::itemUIAdded(MappingUI *)
{
	resized();
}

void StateViewUI::itemUIRemoved(MappingUI *)
{
	resized();
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
