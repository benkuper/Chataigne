/*
  ==============================================================================

    ConditionUI.cpp
    Created: 28 Oct 2016 8:07:05pm
    Author:  bkupe

  ==============================================================================
*/

#include "ConditionUI.h"



ConditionUI::ConditionUI(Condition * _condition) :
	BaseItemUI<Condition>(_condition),
	condition(_condition)
{
	autoSelectWithChildRespect = false;
	targetUI = condition->sourceTarget->createTargetUI();
	condition->addConditionListener(this);
	addAndMakeVisible(targetUI);
	setSize(100, 50);
	updateSourceUI();
}

ConditionUI::~ConditionUI()
{
	condition->removeConditionListener(this);
}


void ConditionUI::resized()
{
	BaseItemUI::resized();

	Rectangle<int> r = getLocalBounds().reduced(2);
	r.removeFromTop(headerHeight + headerGap);

	Rectangle<int> sr = r.withHeight(headerHeight);
	if (sourceFeedbackUI != nullptr)
	{
		sourceFeedbackUI->setBounds(sr.removeFromRight(jmax<int>(sr.getWidth() / 3, 60)));
		sr.removeFromRight(2);
	}
	targetUI->setBounds(sr);

	r.translate(0, headerHeight);

	if (comparatorUI != nullptr)
	{
		comparatorUI->setBounds(r.withHeight(comparatorUI->getHeight()));
		if (comparatorUI->getBottom() != getHeight()) setSize(getWidth(), comparatorUI->getBottom());
	} 

}

void ConditionUI::paintOverChildren(Graphics & g)
{
	BaseItemUI::paintOverChildren(g);
	if (condition->isValid->boolValue())
	{
		g.setColour(condition->enabled->boolValue()?GREEN_COLOR:LIGHTCONTOUR_COLOR);
		g.drawRoundedRectangle(getLocalBounds().toFloat(), 2, 2);
	}
}

void ConditionUI::updateSourceUI()
{
	if (sourceFeedbackUI != nullptr) removeChildComponent(sourceFeedbackUI);
	if (condition->sourceControllable != nullptr)
	{
		sourceFeedbackUI = condition->sourceControllable->createDefaultUI();
		sourceFeedbackUI->setForceFeedbackOnly(true);
		addAndMakeVisible(sourceFeedbackUI);
	}

	if (comparatorUI != nullptr)
	{
		removeChildComponent(comparatorUI);
		comparatorUI = nullptr;
	}
	if (condition->comparator != nullptr)
	{
		comparatorUI = condition->comparator->createUI();
		addAndMakeVisible(comparatorUI);
	}

	resized();
}

void ConditionUI::conditionSourceChanged(Condition *)
{
	updateSourceUI();
}

void ConditionUI::conditionValidationChanged(Condition *)
{
	repaint();
}

void ConditionUI::childBoundsChanged(Component *)
{
	resized();
}
