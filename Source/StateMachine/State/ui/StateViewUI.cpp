/*
  ==============================================================================

    StateViewUI.cpp
    Created: 28 Oct 2016 8:27:57pm
    Author:  bkupe

  ==============================================================================
*/

#include "StateViewUI.h"

StateViewUI::StateViewUI(State * state) :
	BaseItemUI(state, Direction::ALL, true),
	pmui(&state->pm),
	transitionReceptionMode(NONE)
{
	activeUI.reset(state->active->createToggle());

	addAndMakeVisible(activeUI.get());

	pmui.addManagerUIListener(this);
	addAndMakeVisible(&pmui);
	contentComponents.add(&pmui);

	autoHideWhenDragging = false;
	drawEmptyDragIcon = true;

	updateMiniModeUI();
}
	

StateViewUI::~StateViewUI()
{
}

void StateViewUI::setTransitionReceptionMode(TransitionReceptionMode value)
{
	transitionReceptionMode = value;
	pmui.setEnabled(transitionReceptionMode == NONE);
	setRepaintsOnMouseActivity(transitionReceptionMode != NONE);
}

void StateViewUI::mouseDown(const MouseEvent & e)
{
	BaseItemUI::mouseDown(e);

	if (transitionReceptionMode == START)
	{
		stateEditorListeners.call(&StateViewUI::Listener::askCreateTransitionFromUI, this);
	}
	if (transitionReceptionMode == FINISH)
	{
		stateEditorListeners.call(&StateViewUI::Listener::askFinishTransitionFromUI, this);
	} else
	{
		if (e.mods.isLeftButtonDown())
		{
			if (e.originalComponent == &pmui) item->selectThis();
		}else if (e.mods.isRightButtonDown())
		{
			if (e.originalComponent != &pmui 
				&& e.originalComponent->findParentComponentOfClass<ProcessorManagerUI>() == nullptr
				&& e.originalComponent != activeUI.get() && e.originalComponent != enabledBT.get())
			{
				PopupMenu p;
				p.addItem(1, "Create Transition from here");
				p.addItem(2, item->miniMode->boolValue()?"Maximize":"Minimize");

				int result = p.show();
				switch (result)
				{
				case 1:
					stateEditorListeners.call(&StateViewUI::Listener::askCreateTransitionFromUI, this);
					break;
				
				case 2:
					item->miniMode->setValue(!item->miniMode->boolValue());
					break;
				}
			}
		}

	}	
}

void StateViewUI::mouseDoubleClick(const MouseEvent& e)
{
	BaseItemUI::mouseDoubleClick(e);

	if (e.originalComponent == this)
	{
		item->miniMode->setValue(!item->miniMode->boolValue());
	}
}

bool StateViewUI::isUsingMouseWheel()
{
	return pmui.viewport.getVerticalScrollBar().isVisible();// < pmui.container.getHeight();
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

