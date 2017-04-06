/*
  ==============================================================================

    SequenceChooserUI.cpp
    Created: 20 Feb 2017 4:16:00pm
    Author:  Ben

  ==============================================================================
*/

#include "SequenceChooserUI.h"
#include "SequenceManager.h"
#include "SequenceLayer.h"
#include "TimeCueManager.h"
#include "Sequence.h"

SequenceChooserUI::SequenceChooserUI(TargetParameter * p, TargetType _type) :
	TargetParameterUI(p),
	targetType(_type),
	sequenceForCue(nullptr)
{
	updateLabel();
}

SequenceChooserUI::~SequenceChooserUI()
{
}

void SequenceChooserUI::updateLabel()
{
	DBG("Update label " << (int)(targetParameter->targetContainer != nullptr));
	if (targetParameter->targetContainer != nullptr)
	{
		String targetName = targetParameter->targetContainer->niceName;
		if (targetType == LAYER)
		{
			targetName = targetParameter->targetContainer->parentContainer->parentContainer->niceName + ":" + targetName;
		}
		else if (targetType == CUE)
		{
			targetName = targetParameter->targetContainer->parentContainer->parentContainer->niceName + ":" + targetName;
		}


		label.setText(targetName, dontSendNotification);
	} else
	{
		TargetParameterUI::updateLabel();
	}
}

void SequenceChooserUI::showPopupAndGetTarget()
{

	PopupMenu p;
	switch (targetType)
	{
	case SEQUENCE:
		p = SequenceManager::getInstance()->getAllSequencesMenu();
		break;

	case LAYER:
		p = SequenceManager::getInstance()->getAllLayersMenu();
		break;

	case CUE:
		if (sequenceForCue != nullptr) p = sequenceForCue->cueManager->getItemsMenu(1);
		break;
	}

	int result = p.show();

	ControllableContainer * c = nullptr;

	switch (targetType)
	{
	case SEQUENCE:
		c = SequenceManager::getInstance()->getSequenceForItemID(result);
		break;

	case LAYER:
		c = SequenceManager::getInstance()->getLayerForItemID(result);
		break;

	case CUE:
		if (sequenceForCue != nullptr) c = sequenceForCue->cueManager->getItemForMenuResultID(result, 1);
		break;

	}

	if (c != nullptr) ((TargetParameter *)parameter.get())->setValueFromTarget(c);
}

