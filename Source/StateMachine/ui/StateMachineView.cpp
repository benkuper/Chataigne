/*
  ==============================================================================

    StateMachineView.cpp
    Created: 28 Oct 2016 8:27:44pm
    Author:  bkupe

  ==============================================================================
*/

#include "StateMachineView.h"

StateMachineView::StateMachineView(const String &contentName, StateManager * _manager) :
	BaseManagerShapeShifterViewUI(contentName, _manager),
	manager(_manager),
	transitionCreationMode(false),
	transitionCreationSourceUI(nullptr)
{
	canZoom = false;
	contentIsFlexible = true;
	addItemText = "Add State";
	noItemText = "Add interaction by creating a State";
	

	stmUI = new StateTransitionManagerUI(this, &manager->stm);
	addAndMakeVisible(stmUI, 0);
	

	addExistingItems();
	frameView();
}

StateMachineView::~StateMachineView()
{
	
}


void StateMachineView::mouseMove(const MouseEvent & e)
{
	BaseManagerViewUI::mouseMove(e);
	if(transitionCreationMode) repaint();
}

void StateMachineView::mouseDown(const MouseEvent & e)
{
	if (transitionCreationMode && e.eventComponent == this)
	{
		cancelCreateTransition();
		return;
	}

	BaseManagerShapeShifterViewUI::mouseDown(e);
}

void StateMachineView::mouseDoubleClick(const MouseEvent & e)
{
	BaseManagerShapeShifterViewUI::mouseDoubleClick(e);
	manager->addItem(getViewPos(e.getMouseDownPosition()).toFloat());
}

bool StateMachineView::keyPressed(const KeyPress & e)
{

	if (e.isKeyCode(KeyPress::escapeKey) && transitionCreationMode)
	{
		cancelCreateTransition();
		return true;
	}

	return BaseManagerShapeShifterViewUI::keyPressed(e);
}

void StateMachineView::paint(Graphics & g)
{
	BaseManagerViewUI::paint(g);

	if (transitionCreationMode)
	{
		Point<int> sourceP = transitionCreationSourceUI->getBounds().getConstrainedPoint(getMouseXYRelative());
		g.setColour(HIGHLIGHT_COLOR);
		Line<float> line(sourceP.x, sourceP.y, getMouseXYRelative().x, getMouseXYRelative().y);
		float dl[] = {5,5};
		g.drawDashedLine(line, dl, 2);
	}
}


void StateMachineView::resized()
{
	BaseManagerViewUI::resized();
	stmUI->setBounds(getLocalBounds());
	stmUI->resized();
}


void StateMachineView::startCreateTransition(StateViewUI * sourceUI)
{
	transitionCreationMode = true;
	transitionCreationSourceUI = sourceUI;

	for (auto &sui : itemsUI)
	{
		sui->setTransitionReceptionMode(true);
	}
}

void StateMachineView::finishCreateTransition(StateViewUI * destUI)
{
	transitionCreationMode = false;

	if (transitionCreationSourceUI != nullptr && destUI != nullptr && destUI != transitionCreationSourceUI)
	{
		manager->stm.addItem(transitionCreationSourceUI->item, destUI->item);
	}

	for (auto &sui : itemsUI)
	{
		sui->setTransitionReceptionMode(false);
	}

	repaint();
}

void StateMachineView::cancelCreateTransition()
{
	transitionCreationMode = false;
	transitionCreationSourceUI = nullptr;
	for (auto &sui : itemsUI)
	{
		sui->setTransitionReceptionMode(false);
	}

	repaint();
}

void StateMachineView::addItemUIInternal(StateViewUI * se)
{
	BaseManagerViewUI::addItemUIInternal(se);
	se->addStateViewUIListener(this);
}


void StateMachineView::removeItemUIInternal(StateViewUI * se)
{
	BaseManagerViewUI::removeItemUIInternal(se);
	se->removeStateViewUIListener(this);
}


void StateMachineView::askCreateTransitionFromUI(StateViewUI * se)
{
	startCreateTransition(se);
}

void StateMachineView::askFinishTransitionFromUI(StateViewUI * se)
{
	finishCreateTransition(se);
}