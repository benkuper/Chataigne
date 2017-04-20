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
	resizer(this,nullptr),
	transitionReceptionMode(false)
{
	activeUI = state->active->createToggle();
	permanentUI = state->permanent->createToggle();
	
	addAndMakeVisible(activeUI);
	addAndMakeVisible(permanentUI);

	amui.drawContour = true;
	mmui.drawContour = true;
	amui.bgColor = ACTION_COLOR.withSaturation(.2f).darker(1);
	mmui.bgColor = MAPPING_COLOR.withSaturation(.2f).darker(1);
	amui.addManagerUIListener(this);
	mmui.addManagerUIListener(this);

	addAndMakeVisible(&grabber);

	contentContainer.addAndMakeVisible(&amui);
	contentContainer.addAndMakeVisible(&mmui);

	//force color here
	bgColor = item->active->boolValue() ? (item->permanent->boolValue() ? GREEN_COLOR : FEEDBACK_COLOR).darker(.5f) : BG_COLOR.brighter(.1f);

	setSize(item->viewUISize->getPoint().x,item->viewUISize->getPoint().y);
	updateMiniModeUI();
	
}

StateViewUI::~StateViewUI()
{
}

void StateViewUI::setTransitionReceptionMode(bool value)
{
	transitionReceptionMode = value;
	amui.setEnabled(!transitionReceptionMode);
	mmui.setEnabled(!transitionReceptionMode);
	grabber.setEnabled(!transitionReceptionMode);
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
	
	stateEditorListeners.call(&Listener::editorMiniModeChanged, this);
}

void StateViewUI::mouseDown(const MouseEvent & e)
{
	BaseItemUI<State>::mouseDown(e);

	if (transitionReceptionMode)
	{
		stateEditorListeners.call(&Listener::askFinishTransitionFromUI, this);
	} else
	{
		if (e.mods.isLeftButtonDown())
		{
			if (e.eventComponent == &grabber) posAtMouseDown = item->viewUIPosition->getPoint();
		} else if (e.mods.isRightButtonDown())
		{
			PopupMenu p;
			p.addItem(1, "Create Transition");
			int result = p.show();
			switch (result)
			{
			case 1:
				stateEditorListeners.call(&Listener::askCreateTransitionFromUI, this);
				break;
			}
		}
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

void StateViewUI::mouseDoubleClick(const MouseEvent &e)
{
	if (e.eventComponent == &grabber) item->miniMode->setValue(!item->miniMode->boolValue());
}

void StateViewUI::paintOverChildren(Graphics & g)
{
	BaseItemUI::paintOverChildren(g);

	if (transitionReceptionMode && isMouseOver(true))
	{
		g.fillAll(HIGHLIGHT_COLOR.withAlpha(.2f));
	}
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
	permanentUI->setBounds(h.removeFromRight(50));
	h.removeFromRight(2);
	activeUI->setBounds(h.removeFromRight(50));
	h.removeFromRight(2);
	nameUI->setBounds(h);

	
	if (!item->miniMode->boolValue())
	{
		contentContainer.setBounds(r);
		Rectangle<int> cr = contentContainer.getLocalBounds();
		int gap = 4;
		int amHeight = jmax<int>(amui.getContentHeight(), 30);
		int mmHeight = jmax<int>(mmui.getContentHeight(), 30);

		item->viewUISize->setPoint(getWidth(), getHeight()+12); //should look into that +12, if not there, size shrinks between saves

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
	else if (c == item->active || c == item->permanent)
	{
		bgColor = item->active->boolValue() ? (item->permanent->boolValue() ? GREEN_COLOR : FEEDBACK_COLOR) : BG_COLOR.brighter(.1f);
		repaint();
	}
}

void StateViewUI::inspectableSelectionChanged(Inspectable * i)
{
	BaseItemUI::inspectableSelectionChanged(i);
	stateEditorListeners.call(&Listener::editorSelectionChanged, this);
}

//GRABBER

void StateViewUI::Grabber::paint(Graphics & g)
{
	Rectangle<int> r = getLocalBounds();
	g.setColour(BG_COLOR.brighter(.3f));
	const int numLines = 3;
	for (int i = 0; i < numLines; i++)
	{
		float th = (i + 1)*(float)getHeight() / ((float)numLines + 1);
		g.drawLine(0, th, (float)getWidth(), th, 1);
	}
}
