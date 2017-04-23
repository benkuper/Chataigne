/*
  ==============================================================================

    DashboardItemUI.cpp
    Created: 19 Apr 2017 11:01:13pm
    Author:  Ben

  ==============================================================================
*/

#include "DashboardItemUI.h"

DashboardItemUI::DashboardItemUI(DashboardItem * item) :
	BaseItemUI(item),
	currentControllable(nullptr)
{
	setControllableUI(item->target->target);
	setSize(100, 100);
}

DashboardItemUI::~DashboardItemUI()
{
	setControllableUI(nullptr);
}

void DashboardItemUI::setControllableUI(Controllable * c)
{
	if (c == currentControllable) return;

	if (currentControllable != nullptr)
	{
		currentControllable->removeInspectableListener(this);
		removeChildComponent(targetUI);
		targetUI = nullptr;
	}

	currentControllable = c;

	if (currentControllable != nullptr)
	{
		currentControllable->addInspectableListener(this);
		targetUI = currentControllable->createDefaultUI();
		addAndMakeVisible(targetUI);
	}
	resized();
}

void DashboardItemUI::resizedInternalContent(Rectangle<int>& r)
{
	if (targetUI != nullptr) targetUI->setBounds(r);
}

void DashboardItemUI::inspectableDestroyed(Inspectable * i)
{
	BaseItemUI::inspectableDestroyed(i);
	if (i == currentControllable) setControllableUI(nullptr);
}

void DashboardItemUI::controllableFeedbackUpdateInternal(Controllable * c)
{
	BaseItemUI::controllableFeedbackUpdateInternal(c);
	if (c == item->target)
	{
		setControllableUI(item->target->target);
	}
}
