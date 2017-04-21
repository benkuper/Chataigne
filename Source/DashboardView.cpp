/*
  ==============================================================================

    DashboardView.cpp
    Created: 19 Apr 2017 10:57:12pm
    Author:  Ben

  ==============================================================================
*/

#include "DashboardView.h"

DashboardView::DashboardView(DashboardItemManager * manager) :
	BaseManagerUI("Dashboard",manager,false)
{
	addItemText = "Add Item";
	setWantsKeyboardFocus(true);

	addExistingItems(false);

	resized();
}

DashboardView::~DashboardView()
{
}

void DashboardView::paint(Graphics & g)
{
	int checkerSize = 32;
	int checkerTX = -checkerSize * 2 + ((getWidth() / 2 + viewOffset.x) % (checkerSize * 2));
	int checkerTY = -checkerSize * 2 + ((getHeight() / 2 + viewOffset.y) % (checkerSize * 2));
	Rectangle<int> checkerBounds(checkerTX, checkerTY, getWidth() + checkerSize * 4, getHeight() + checkerSize * 4);
	g.fillCheckerBoard(checkerBounds, checkerSize, checkerSize, BG_COLOR.darker(.3f), BG_COLOR.darker(.2f));

	g.setColour(BG_COLOR.darker(.05f));
	Point<int> center = getSize() / 2;
	center += viewOffset;
	g.drawLine(center.x, 0, center.x, getHeight(), 2);
	g.drawLine(0, center.y, getWidth(), center.y, 2);
}


void DashboardView::resized()
{
	Rectangle<int> r = getLocalBounds();

	addItemBT->setBounds(r.withSize(24, 24).withX(r.getWidth() - 24));

	for (auto &tui : itemsUI)
	{
		updateViewUIPosition(tui);
	}
}

void DashboardView::updateViewUIPosition(DashboardItemUI * se)
{
	Point<int> pe = getSize()*se->item->viewUIPosition->getPoint();
	pe += getSize() / 2; //position at center of window
	pe += viewOffset;
	se->setTopLeftPosition(pe.x, pe.y);

}

void DashboardView::addItemFromMenu(bool isFromAddButton, Point<int> mouseDownPos)
{
	manager->addItem(getViewPos(mouseDownPos).toFloat() / getSize());
}

Point<int> DashboardView::getSize()
{
	return Point<int>(getWidth(), getHeight());
}

Point<int> DashboardView::getViewMousePosition()
{
	return getViewPos(getMouseXYRelative());
}

Point<int> DashboardView::getViewPos(const Point<int> &originalPos)
{
	return originalPos - getSize() / 2 - viewOffset;
}

Point<float> DashboardView::getEditorsCenter()
{
	Point<float> average;
	for (auto &se : itemsUI)
	{
		average += se->item->viewUIPosition->getPoint();
	}
	average /= itemsUI.size();
	return average;
}


void DashboardView::homeView()
{
	viewOffset = Point<int>();
	resized();
	repaint();

}

void DashboardView::frameView()
{
	viewOffset = (-getEditorsCenter()*getSize()).toInt();
	resized();
	repaint();
}



void DashboardView::mouseDown(const MouseEvent & e)
{
	BaseManagerUI::mouseDown(e);
	if ((e.mods.isLeftButtonDown() && e.mods.isAltDown()) || e.mods.isMiddleButtonDown())
	{
		setMouseCursor(MouseCursor::UpDownLeftRightResizeCursor);
		updateMouseCursor();
		initViewOffset = Point<int>(viewOffset.x, viewOffset.y);
	}
}

void DashboardView::mouseDrag(const MouseEvent & e)
{
	if ((e.mods.isLeftButtonDown() && e.mods.isAltDown()) || e.mods.isMiddleButtonDown())
	{
		viewOffset = initViewOffset + e.getOffsetFromDragStart();
		resized();
		repaint();
	}
}

void DashboardView::mouseMove(const MouseEvent & e)
{
	//
}

void DashboardView::mouseUp(const MouseEvent & e)
{
	setMouseCursor(MouseCursor::NormalCursor);
	updateMouseCursor();
}

bool DashboardView::keyPressed(const KeyPress & e)
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

DashboardItemUI * DashboardView::createUIForItem(DashboardItem * i)
{
	return i->getItemUI();
}

void DashboardView::addItemUIInternal(DashboardItemUI * se)
{
	updateViewUIPosition(se);
	se->addItemUIListener(this);
}

void DashboardView::removeItemUIInternal(DashboardItemUI * se)
{
	se->removeItemUIListener(this);
}

void DashboardView::itemUIGrabbed(DashboardItemUI * se)
{
	updateViewUIPosition(se);
}
