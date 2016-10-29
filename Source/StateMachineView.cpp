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
	BaseManagerUI("State Machine",_manager),
	manager(_manager)
{
	addItemText = "Add State";
}

StateMachineView::~StateMachineView()
{
}

StateEditor * StateMachineView::addItemUI(State * s)
{
	StateEditor * se = BaseManagerUI<StateManager, State, StateEditor>::addItemUI(s);
	
	if (se->item->editorPosition->x == 0 && se->item->editorPosition->y == 0)
	{
		se->item->editorPosition->setPoint(getMouseXYRelative().toFloat() / Point<float>((float)getWidth(), (float)getHeight()));
	}

	return se;
}

void StateMachineView::paint(Graphics & g)
{
	g.fillCheckerBoard(getLocalBounds(), 16,16, BG_COLOR.darker(.3f), BG_COLOR.darker(.2f));
	/*
	g.setColour(BG_COLOR);
	Point<int> center = getLocalBounds().getCentre();
	g.drawLine(center.x, 0, center.x, getHeight(),2);
	g.drawLine(0, center.y, getWidth(), center.y);
	*/
}

void StateMachineView::resized()
{
	Rectangle<int> r = getLocalBounds();
	Point<int> rSize(r.getWidth(), r.getHeight());
	for (auto &tui : itemsUI)
	{
		tui->setTopLeftPosition((tui->item->editorPosition->getPoint()*rSize).toInt());
	}
}

