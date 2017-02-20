/*
  ==============================================================================

    StateChooserUI.cpp
    Created: 20 Feb 2017 2:39:44pm
    Author:  Ben

  ==============================================================================
*/

#include "StateChooserUI.h"

StateChooserUI::StateChooserUI(TargetParameter * p, TargetType _type) :
	TargetParameterUI(p),
	targetType(_type)
{
	updateLabel();
}

StateChooserUI::~StateChooserUI()
{
}

void StateChooserUI::updateLabel()
{
	if (targetParameter->targetContainer != nullptr)
	{
		String targetName = targetParameter->targetContainer->niceName;
		if (targetType != STATE)
		{
			targetName = targetParameter->targetContainer->parentContainer->parentContainer->niceName + ":" + targetName;
		}
		
		
		label.setText(targetName, dontSendNotification);
	} else
	{
		TargetParameterUI::updateLabel();
	}
}

void StateChooserUI::showPopupAndGetTarget()
{
	
	PopupMenu p;
	switch (targetType)
	{
	case STATE:
		p = StateManager::getInstance()->getAllStatesMenu();
		break;

	case ACTION:
		p = StateManager::getInstance()->getAllActionsMenu();
		break;

	case MAPPING:
		p = StateManager::getInstance()->getAllMappingsMenu();
		break;
	}
	int result = p.show();

	ControllableContainer * c = nullptr;
	switch (targetType)
	{
	case STATE:
		c = StateManager::getInstance()->getStateForItemID(result);
		break;

	case ACTION:
		c = StateManager::getInstance()->getActionForItemID(result);
		break;

	case MAPPING:
		c = StateManager::getInstance()->getMappingForItemID(result);
		break;
	}
	if (c != nullptr) ((TargetParameter *)parameter.get())->setValueFromTarget(c);
}
