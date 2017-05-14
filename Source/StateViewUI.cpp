/*
  ==============================================================================

    StateViewUI.cpp
    Created: 28 Oct 2016 8:27:57pm
    Author:  bkupe

  ==============================================================================
*/

#include "StateViewUI.h"

StateViewUI::StateViewUI(State * state) :
BaseItemUI<State>(state, ResizeMode::ALL, true),
	amui(&state->am),
	mmui(&state->mm),
	transitionReceptionMode(false)
{
	activeUI = state->active->createToggle();

	addAndMakeVisible(activeUI);

	amui.drawContour = true;
	mmui.drawContour = true;
	amui.bgColor = ACTION_COLOR.withSaturation(.2f).darker(1);
	mmui.bgColor = MAPPING_COLOR.withSaturation(.2f).darker(1);
	amui.addManagerUIListener(this);
	mmui.addManagerUIListener(this);

	if (!item->miniMode->boolValue())
	{
		addAndMakeVisible(&amui);
		addAndMakeVisible(&mmui);
	}

	//bgColor = item->active->boolValue() ? (item->permanent->boolValue() ? GREEN_COLOR : FEEDBACK_COLOR) : BG_COLOR.brighter(.1f);

	//setSize(250, 250);

	resized();
}
	

StateViewUI::~StateViewUI()
{
}

void StateViewUI::setTransitionReceptionMode(bool value)
{
	transitionReceptionMode = value;
	amui.setEnabled(!transitionReceptionMode);
	mmui.setEnabled(!transitionReceptionMode);
	grabber->setEnabled(!transitionReceptionMode);
}

void StateViewUI::updateMiniModeUI()
{
	BaseItemUI::updateMiniModeUI();

	if (item->miniMode->boolValue())
	{
		removeChildComponent(&amui);
		removeChildComponent(&mmui);
	} else
	{
		addAndMakeVisible(&amui);
		addAndMakeVisible(&mmui);
	}

}

void StateViewUI::mouseDown(const MouseEvent & e)
{
	BaseItemUI<State>::mouseDown(e);

	if (transitionReceptionMode)
	{
		stateEditorListeners.call(&Listener::askFinishTransitionFromUI, this);
	} else
	{
		 if (e.mods.isRightButtonDown() && e.originalComponent != &amui && e.originalComponent != &mmui)
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


void StateViewUI::paintOverChildren(Graphics & g)
{
	if (item->active->boolValue())
	{
		Colour c = item->permanent->boolValue() ? GREEN_COLOR : FEEDBACK_COLOR;
		g.setColour(c);
		g.drawRoundedRectangle(getLocalBounds().reduced(2).toFloat(), 2, 2);
	}

	BaseItemUI::paintOverChildren(g);


	if (transitionReceptionMode && isMouseOver(true))
	{
		g.fillAll(HIGHLIGHT_COLOR.withAlpha(.2f));
	}
}


void StateViewUI::resizedInternalHeader(Rectangle<int>& r)
{
	activeUI->setBounds(r.removeFromRight(50).reduced(0,2));
	r.removeFromRight(2);
}

void StateViewUI::resizedInternalContent(Rectangle<int>& r)
{
	int gap = 4;
	int amHeight = jmax<int>(amui.getContentHeight(), 30);
	int mmHeight = jmax<int>(mmui.getContentHeight(), 30);


	if ((amHeight > r.getHeight() / 2 || mmHeight > r.getHeight() / 2) && amHeight + mmHeight + gap <= r.getHeight())
	{
		if (amHeight > mmHeight)
		{
			amui.setBounds(r.removeFromTop(amHeight));
			r.removeFromTop(gap);
			mmui.setBounds(r);
		} else
		{
			mmui.setBounds(r.removeFromBottom(mmHeight));
			r.removeFromBottom(gap);
			amui.setBounds(r);
		}
	} else
	{
		amui.setBounds(r.removeFromTop((r.getHeight() - gap) / 2));
		r.removeFromTop(gap);
		mmui.setBounds(r);
	}
}

void StateViewUI::childBoundsChanged(Component * c)
{
	BaseItemUI::childBoundsChanged(c);
	resized();
}

void StateViewUI::controllableFeedbackUpdateInternal(Controllable * c)
{
	BaseItemUI::controllableFeedbackUpdateInternal(c);

	if (c == item->active || c == item->permanent)
	{
		//bgColor = item->active->boolValue() ? (item->permanent->boolValue() ? GREEN_COLOR : FEEDBACK_COLOR) : BG_COLOR.brighter(.1f);
		repaint();
	}
}

void StateViewUI::inspectableSelectionChanged(Inspectable * i)
{
	BaseItemUI::inspectableSelectionChanged(i);
	stateEditorListeners.call(&Listener::editorSelectionChanged, this);
}
