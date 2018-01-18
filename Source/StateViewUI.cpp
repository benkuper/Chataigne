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
	pmui(&state->pm),
	transitionReceptionMode(false)
{
	activeUI = state->active->createToggle();

	addAndMakeVisible(activeUI);

	pmui.drawContour = true;
	pmui.addManagerUIListener(this);

	if (!item->miniMode->boolValue())
	{
		addAndMakeVisible(&pmui);
	}

	setSize(200,200);
	resized();
}
	

StateViewUI::~StateViewUI()
{
}

void StateViewUI::setTransitionReceptionMode(bool value)
{
	transitionReceptionMode = value;
	pmui.setEnabled(!transitionReceptionMode);
	grabber->setEnabled(!transitionReceptionMode);
}

void StateViewUI::updateMiniModeUI()
{
	BaseItemUI::updateMiniModeUI();

	if (item->miniMode->boolValue())
	{
		removeChildComponent(&pmui);
	} else
	{
		addAndMakeVisible(&pmui);
	}

}

void StateViewUI::mouseDown(const MouseEvent & e)
{
	BaseItemUI<State>::mouseDown(e);


	if (transitionReceptionMode)
	{
		stateEditorListeners.call(&StateViewUI::Listener::askFinishTransitionFromUI, this);
	} else
	{
		if (e.mods.isLeftButtonDown())
		{
			if (e.originalComponent == &pmui) item->selectThis();
		}else if (e.mods.isRightButtonDown())
		{
			if (e.originalComponent != &pmui)
			{
				PopupMenu p;
				p.addItem(1, "Create Transition");
				int result = p.show();
				switch (result)
				{
				case 1:
					stateEditorListeners.call(&StateViewUI::Listener::askCreateTransitionFromUI, this);
					break;
				}
			}
		}

	}	
}


void StateViewUI::paint(Graphics & g)
{
	BaseItemUI::paint(g);
	if (item->active->boolValue())
	{
		g.setColour(FEEDBACK_COLOR);
		g.drawRoundedRectangle(getLocalBounds().reduced(1).toFloat(), 2 ,1);
	}

}

void StateViewUI::paintOverChildren(Graphics & g)
{
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
	pmui.setBounds(r);
}

void StateViewUI::childBoundsChanged(Component * c)
{
	BaseItemUI::childBoundsChanged(c);
	resized();
}

void StateViewUI::controllableFeedbackUpdateInternal(Controllable * c)
{
	BaseItemUI::controllableFeedbackUpdateInternal(c);

	if (c == item->active)
	{
		//bgColor = item->active->boolValue() ? (item->permanent->boolValue() ? GREEN_COLOR : FEEDBACK_COLOR) : BG_COLOR.brighter(.1f);
		repaint();
	}
}

void StateViewUI::inspectableSelectionChanged(Inspectable * i)
{
	BaseItemUI::inspectableSelectionChanged(i);
	stateEditorListeners.call(&StateViewUI::Listener::editorSelectionChanged, this);
}

