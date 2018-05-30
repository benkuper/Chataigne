/*
  ==============================================================================

    ActionUI.cpp
    Created: 28 Oct 2016 8:05:24pm
    Author:  bkupe

  ==============================================================================
*/

#include "ActionUI.h"

ActionUI::ActionUI(Action * _action) :
	ProcessorUI(_action),
	action(_action)
{
	action->addAsyncActionListener(this);

	

	/*
	validUI = action->cdm.isValid->createToggle();
	validUI->showLabel = false;
	validUI->showValue = false;
	addAndMakeVisible(validUI);
	*/

	triggerAllUI = action->csmOn->triggerAll->createButtonUI();
	addAndMakeVisible(triggerAllUI);

	updateRoleBGColor();
}

ActionUI::~ActionUI()
{
	if (!inspectable.wasObjectDeleted()) action->removeAsyncActionListener(this);
}

void ActionUI::updateRoleBGColor()
{
	switch (action->actionRole)
	{
	case Action::STANDARD:
		bgColor = ACTION_COLOR.withSaturation(.4f).darker(1);
		break;
	case Action::ACTIVATE:
		bgColor = GREEN_COLOR.withSaturation(.4f).darker(1);
		break;
	case Action::DEACTIVATE:
		bgColor = RED_COLOR.withSaturation(.4f).darker(1);
		break;
	}
}

void ActionUI::resizedInternalHeader(Rectangle<int>& r)
{
	BaseItemUI::resizedInternalHeader(r);
	//validUI->setBounds(r.removeFromRight(headerHeight));
	//r.removeFromRight(2);
	triggerAllUI->setBounds(r.removeFromRight(60));
}

void ActionUI::paintOverChildren(Graphics & g)
{
	BaseItemUI::paintOverChildren(g);
	if (action->cdm.isValid->boolValue() && action->actionRole == Action::STANDARD)
	{
		g.setColour(GREEN_COLOR);
		g.drawRoundedRectangle(getMainBounds().toFloat(), rounderCornerSize, 2);
	}
}

void ActionUI::newMessage(const Action::ActionEvent & e)
{
	switch (e.type)
	{
	case Action::ActionEvent::ENABLED_CHANGED:
		break;

	case Action::ActionEvent::ROLE_CHANGED:
		updateRoleBGColor();
		repaint();
		break;

	case Action::ActionEvent::VALIDATION_CHANGED:
		if(action->actionRole == Action::STANDARD) repaint();
		break;

	}
}
