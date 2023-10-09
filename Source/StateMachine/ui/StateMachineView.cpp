/*
  ==============================================================================

    StateMachineView.cpp
    Created: 28 Oct 2016 8:27:44pm
    Author:  bkupe

  ==============================================================================
*/
#include "StateMachine/StateMachineIncludes.h"

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
	
	stmUI.reset(new StateTransitionManagerUI(this, &manager->stm));
	addAndMakeVisible(stmUI.get(), 0);
	
	commentManagerUI.reset(new CommentManagerViewUI(&manager->commentManager));
	commentManagerUI->canZoom = true;
	commentManagerUI->addExistingItems();
	addAndMakeVisible(commentManagerUI.get(), 0);

	updatePositionOnDragMove = true;

	enableSnapping = true;
	
	setShowPane(true);

	setViewZoom(manager->viewZoom);
	//frameView();

	setShowTools(true);
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
	if(e.mods.isLeftButtonDown()) manager->addItem(getViewPos(e.getMouseDownPosition()).toFloat());
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
		commentManagerUI->manager->viewOffset = manager->viewOffset;
		commentManagerUI->setBounds(getLocalBounds());
		commentManagerUI->resized();
	}
}

void StateMachineView::setViewZoom(float value)
{
	BaseManagerViewUI::setViewZoom(value);
	if(commentManagerUI != nullptr) commentManagerUI->setViewZoom(value);
}

void StateMachineView::updateItemsVisibility()
{
	BaseManagerViewUI::updateItemsVisibility();
	if(commentManagerUI != nullptr) commentManagerUI->updateItemsVisibility();
}

void StateMachineView::showMenuAndAddItem(bool fromAddButton, Point<int> position)
{
	PopupMenu p;
	p.addItem(1, "Add State");
	p.addItem(2, "Add Comment");
	p.addItem(3, "Create Transition", manager->items.size() >= 2);
	p.addItem(4, "Snapping",true, enableSnapping);
	p.addItem(5, showTools ? "Hide Tools" : "Show Tools");

	p.showMenuAsync(PopupMenu::Options(), [this, fromAddButton, position](int result)
		{
			if (result == 1)	BaseManagerViewUI::addItemFromMenu(fromAddButton, position);
			else if (result == 2) commentManagerUI->addItemFromMenu(fromAddButton, position);
			else if (result == 3) startCreateTransition(nullptr);
			else if (result == 4) enableSnapping = !enableSnapping;
			else if (result == 5) setShowTools(!showTools);
		}
	);
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
		manager->stm.addItem(manager->stm.createItem(transitionCreationSourceUI->item, destUI->item));
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
