/*
  ==============================================================================

    StateMachineView.cpp
    Created: 28 Oct 2016 8:27:44pm
    Author:  bkupe

  ==============================================================================
*/

#include "StateMachineView.h"
#include "AssetManager.h"

StateMachineView::StateMachineView(StateManager * _manager) :
	BaseManagerShapeShifterUI("State Machine", _manager, false),
	manager(_manager)
{
	contentIsFlexible = true;
	addItemText = "Add State";
	setWantsKeyboardFocus(true);
}

StateMachineView::~StateMachineView()
{
	
}

void StateMachineView::mouseDown(const MouseEvent & e)
{
	BaseManagerUI::mouseDown(e);
	if (e.mods.isAltDown())
	{
		setMouseCursor(MouseCursor::UpDownLeftRightResizeCursor);
		updateMouseCursor();
		initViewOffset = Point<int>(viewOffset.x, viewOffset.y);
	}
}

void StateMachineView::mouseDrag(const MouseEvent & e)
{
	if (e.mods.isAltDown())
	{
		viewOffset = initViewOffset + e.getOffsetFromDragStart();
		resized();
		repaint();
	}
}

void StateMachineView::mouseUp(const MouseEvent & e)
{
	setMouseCursor(MouseCursor::NormalCursor);
	updateMouseCursor();
}

bool StateMachineView::keyPressed(const KeyPress & e)
{
	if (e.getKeyCode() == KeyPress::createFromDescription("f").getKeyCode())
	{
		frameView();
		return true;
	} else if (e.getKeyCode() == KeyPress::createFromDescription("h").getKeyCode())
	{
		homeView();
		return true;
	}

	return false;
}

void StateMachineView::paint(Graphics & g)
{
	int checkerSize = 32;
	int checkerTX = -checkerSize*2 + ((getWidth()/2+viewOffset.x) % (checkerSize*2));
	int checkerTY = -checkerSize*2 + ((getHeight()/2+viewOffset.y) % (checkerSize*2));
	Rectangle<int> checkerBounds(checkerTX, checkerTY, getWidth() + checkerSize*4, getHeight() + checkerSize*4);
	g.fillCheckerBoard(checkerBounds,checkerSize,checkerSize, BG_COLOR.darker(.3f), BG_COLOR.darker(.2f));
	
	g.setColour(BG_COLOR.darker(.05f));
	Point<int> center = getSize() / 2;
	center += viewOffset;
	g.drawLine(center.x, 0, center.x, getHeight(),2);
	g.drawLine(0, center.y, getWidth(), center.y,2);
	
	
}

void StateMachineView::resized()
{
	Rectangle<int> r = getLocalBounds();

	for (auto &tui : itemsUI)
	{
		updateViewUIPosition(tui);
	}
}

void StateMachineView::updateViewUIPosition(StateViewUI * se)
{
	Point<int> pe = getSize()*se->item->viewUIPosition->getPoint();
	pe += getSize() / 2; //position at center of window
	pe += viewOffset;
	se->setTopLeftPosition(pe.x,pe.y);
}

void StateMachineView::addItemFromMenu()
{
	manager->addItem(getViewMousePosition().toFloat() / getSize());
}

Point<int> StateMachineView::getSize()
{
	return Point<int>(getWidth(), getHeight());
}

Point<int> StateMachineView::getViewMousePosition()
{
	return getMouseXYRelative() - getSize() / 2 - viewOffset;
}

Point<float> StateMachineView::getEditorsCenter()
{
	Point<float> average;
	for (auto &se : itemsUI)
	{
		average += se->item->viewUIPosition->getPoint();
	}
	average /= itemsUI.size();
	return average;
}

void StateMachineView::homeView()
{
	viewOffset = Point<int>();
	resized(); 
	repaint();
	
}

void StateMachineView::frameView()
{
	viewOffset = (-getEditorsCenter()*getSize()).toInt();
	resized();
	repaint();
}

void StateMachineView::addItemUIInternal(StateViewUI * se)
{
	se->addStateViewUIListener(this);
}

void StateMachineView::removeItemUIInternal(StateViewUI * se)
{
	se->removeStateViewUIListener(this);
}

void StateMachineView::editorGrabbed(StateViewUI * se)
{
	updateViewUIPosition(se);
}

