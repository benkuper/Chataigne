/*
  ==============================================================================

    StateMachineView.cpp
    Created: 28 Oct 2016 8:27:44pm
    Author:  bkupe

  ==============================================================================
*/

#include "StateMachineView.h"

#include "../Transition/ui/StateTransitionManagerUI.h"

StateMachineView::StateMachineView(const String &contentName, StateManager * _manager) :
	BaseManagerShapeShifterViewUI(contentName, _manager),
	manager(_manager),
	stmUI(nullptr),
	transitionCreationMode(false),
	transitionCreationSourceUI(nullptr),
    commentManagerUI(nullptr)
{
	canZoom = true;
	contentIsFlexible = true;
	addItemText = "Add State";
	noItemText = "Add interaction by creating a State";

	acceptedDropTypes.add("Comment");

	addExistingItems();
	
	stmUI = new StateTransitionManagerUI(this, &manager->stm);
	addAndMakeVisible(stmUI, 0);
	
	commentManagerUI = new CommentManagerViewUI(&manager->commentManager);
	commentManagerUI->canZoom = true;
	addAndMakeVisible(commentManagerUI, 0);

	frameView();
}

StateMachineView::~StateMachineView()
{
	
}

void StateMachineView::mouseMove(const MouseEvent & e)
{
	BaseManagerViewUI::mouseMove(e);
	if(transitionCreationMode && transitionCreationSourceUI != nullptr) repaint();
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

	if (transitionCreationMode && transitionCreationSourceUI != nullptr)
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

	if (stmUI != nullptr)
	{
		stmUI->setBounds(getLocalBounds());
		stmUI->resized();
	}
	
	if (commentManagerUI != nullptr)
	{
		commentManagerUI->viewOffset = viewOffset;
		commentManagerUI->setBounds(getLocalBounds());
		commentManagerUI->resized();
	}
	
}

void StateMachineView::setViewZoom(float value)
{
	BaseManagerViewUI::setViewZoom(value);
	commentManagerUI->setViewZoom(value);
}

void StateMachineView::showMenuAndAddItem(bool fromAddButton, Point<int> position)
{
	PopupMenu p;
	p.addItem(1, "Add State");
	p.addItem(2, "Add Comment");
	p.addItem(3, "Create Transition", manager->items.size() >= 2);

	int result = p.show();
	if (result == 1)	BaseManagerViewUI::addItemFromMenu(fromAddButton, position);
	else if (result == 2) commentManagerUI->addItemFromMenu(fromAddButton, position);
	else if (result == 3) startCreateTransition(nullptr);
}


void StateMachineView::startCreateTransition(StateViewUI * sourceUI)
{
	transitionCreationMode = true;
	transitionCreationSourceUI = sourceUI;

	//DBG("Start create transition " << (int)(transitionCreationSourceUI != nullptr));

	for (auto &sui : itemsUI)
	{
		if (sui != sourceUI) sui->setTransitionReceptionMode(sourceUI != nullptr ? StateViewUI::FINISH : StateViewUI::START);
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
		sui->setTransitionReceptionMode(StateViewUI::NONE);
	}

	repaint();
}

void StateMachineView::cancelCreateTransition()
{
	transitionCreationMode = false;
	transitionCreationSourceUI = nullptr;
	for (auto &sui : itemsUI)
	{
		sui->setTransitionReceptionMode(StateViewUI::NONE);
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

void StateMachineView::itemDropped(const SourceDetails & details)
{
	String type = details.description.getProperty("type", "").toString();
	if (type == "Comment")
	{
		commentManagerUI->itemDropped(details);
		return;
	}

	BaseManagerViewUI::itemDropped(details);
}
