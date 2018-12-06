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

	triggerAllUI = action->csmOn->triggerAll->createButtonUI();
	addAndMakeVisible(triggerAllUI);

	progressionUI = action->cdm.validationProgress->createSlider();
	progressionUI->showValue = false;
	addChildComponent(progressionUI);
	progressionUI->setVisible(action->cdm.validationProgress->enabled);

	updateRoleBGColor();
}

ActionUI::~ActionUI()
{
	if (!inspectable.wasObjectDeleted()) action->removeAsyncActionListener(this);
}

void ActionUI::updateRoleBGColor()
{
	bool isA = action->actionRoles.contains(Action::ACTIVATE);
	bool isD = action->actionRoles.contains(Action::DEACTIVATE);

	if (isA && isD) bgColor = Colours::orange.withSaturation(.4f).darker(1);
	else if (isA) bgColor = GREEN_COLOR.withSaturation(.4f).darker(1);
	else if (isD) bgColor = RED_COLOR.withSaturation(.4f).darker(1);
	else bgColor = ACTION_COLOR.withSaturation(.4f).darker(1);
}

void ActionUI::controllableFeedbackUpdateInternal(Controllable * c)
{
	ProcessorUI::controllableFeedbackUpdateInternal(c);
	if (c == action->cdm.validationTime)
	{
		bool v = action->cdm.validationProgress->enabled;
		if (progressionUI->isVisible() != v)
		{
			progressionUI->setVisible(v);
			resized();
		}
	}
}

void ActionUI::resizedInternalHeader(Rectangle<int>& r)
{
	BaseItemUI::resizedInternalHeader(r);
	//validUI->setBounds(r.removeFromRight(headerHeight));
	//r.removeFromRight(2);
	triggerAllUI->setBounds(r.removeFromRight(60));
	if (progressionUI->isVisible())
	{
		progressionUI->setBounds(r.removeFromRight(40).reduced(2,6));
	}
}

void ActionUI::paintOverChildren(Graphics & g)
{
	BaseItemUI::paintOverChildren(g);
	if (action->cdm.isValid->boolValue() && action->actionRoles.size() == 0) //no special roles like activate or deactivate
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
		shouldRepaint = true;
		break;

	case Action::ActionEvent::VALIDATION_CHANGED:
		if (action->actionRoles.size() == 0) shouldRepaint = true;
		break;

	}
}
