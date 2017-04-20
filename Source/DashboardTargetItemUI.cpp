/*
  ==============================================================================

    DashboardTargetItemUI.cpp
    Created: 20 Apr 2017 2:24:12pm
    Author:  Ben

  ==============================================================================
*/

#include "DashboardTargetItemUI.h"
#include "TargetParameter.h"

DashboardTargetItemUI::DashboardTargetItemUI(DashboardTargetItem * item) :
	DashboardItemUI(item),
	targetItem(item),
	currentControllable(nullptr)
{
	setControllableUI(targetItem->target->target);
}

DashboardTargetItemUI::~DashboardTargetItemUI()
{
	setControllableUI(nullptr);
}

void DashboardTargetItemUI::setControllableUI(Controllable * c)
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

void DashboardTargetItemUI::resizedInternalContent(Rectangle<int>& r)
{
	if (targetUI != nullptr) targetUI->setBounds(r);
}

void DashboardTargetItemUI::inspectableDestroyed(Inspectable * i)
{
	DashboardItemUI::inspectableDestroyed(i);
	if (i == currentControllable) setControllableUI(nullptr);
}

void DashboardTargetItemUI::controllableFeedbackUpdateInternal(Controllable * c)
{
	DashboardItemUI::controllableFeedbackUpdateInternal(c);
	if (c == targetItem->target)
	{
		setControllableUI(targetItem->target->target);
	}
}
