/*
  ==============================================================================

	ActionUI.cpp
	Created: 28 Oct 2016 8:05:24pm
	Author:  bkupe

  ==============================================================================
*/

#include "ActionUI.h"

#include "Module/ui/ModuleUI.h"
#include "StateMachine/StateManager.h"
#include "Common/Processor/Action/Consequence/Consequence.h"
#include "CustomVariables/CVGroup.h"
#include "../Condition/conditions/StandardCondition/StandardCondition.h"

ActionUI::ActionUI(Action* _action) :
	ProcessorUI(_action),
	action(_action)
{
	acceptedDropTypes.add("Module");
	acceptedDropTypes.add("CommandTemplate");

	action->addAsyncActionListener(this);

	//triggerAllUI.reset(action->csmOn->triggerAll->createButtonUI());
	//addAndMakeVisible(triggerAllUI.get());

	//progressionUI.reset(action->cdm.validationProgress->createSlider());
	//progressionUI->showValue = false;
	//addChildComponent(progressionUI.get());
	//progressionUI->setVisible(action->cdm.validationProgress->enabled);

	updateBGColor();
}

ActionUI::~ActionUI()
{
	if (!inspectable.wasObjectDeleted()) action->removeAsyncActionListener(this);
}

void ActionUI::paint(Graphics & g)
{
	BaseItemUI::paint(g);

	if (isDraggingOver)
	{
		g.setColour(BLUE_COLOR.darker());
		g.fillRoundedRectangle(getLocalBounds().toFloat(), 2);
	}
}

void ActionUI::updateBGColor()
{
	bool isA = action->actionRoles.contains(Action::ACTIVATE);
	bool isD = action->actionRoles.contains(Action::DEACTIVATE);

	if (isA && isD) baseBGColor = Colours::orange.darker(1);
	else if (isA) baseBGColor = GREEN_COLOR.darker(1);
	else if (isD) baseBGColor = Colours::orange.darker(1);
	else baseBGColor = ACTION_COLOR.darker(1);

	ProcessorUI::updateBGColor();
}

void ActionUI::controllableFeedbackUpdateInternal(Controllable * c)
{
	ProcessorUI::controllableFeedbackUpdateInternal(c);
	if (c == action->cdm.validationTime)
	{
		//bool v = action->cdm.validationTime->floatValue() > 0;
		//if (progressionUI->isVisible() != v)
		//{
		//	progressionUI->setVisible(v);
		//	resized();
		//}
	}
}

void ActionUI::resizedInternalHeader(Rectangle<int>& r)
{
	BaseItemUI::resizedInternalHeader(r);

	//triggerAllUI->setBounds(r.removeFromRight(70));
	//if (progressionUI->isVisible())
	//{
	//	progressionUI->setBounds(r.removeFromRight(40).reduced(2, 6));
	//}
}

void ActionUI::paintOverChildren(Graphics & g)
{
	BaseItemUI::paintOverChildren(g);
	if (action->cdm.getIsValid() && action->actionRoles.size() == 0) //no special roles like activate or deactivate
	{
		g.setColour(GREEN_COLOR);
		g.drawRoundedRectangle(getMainBounds().toFloat(), rounderCornerSize, 2);
	}
}

void ActionUI::itemDropped(const SourceDetails & details)
{
	BaseItemUI::itemDropped(details);

	String dataType = details.description.getProperty("dataType", "");
	CommandDefinition * def = nullptr;
	bool isInput = false;
	bool isConsequenceTrue = true;

	if (dataType == "Module")
	{
		ModuleUI * mui = dynamic_cast<ModuleUI *>(details.sourceComponent.get());

		PopupMenu pm;
		ControllableChooserPopupMenu actionInputMenu(&mui->item->valuesCC, 0);

		PopupMenu actionCommandMenuTrue = mui->item->getCommandMenu(20000, CommandContext::ACTION);
		PopupMenu actionCommandMenuFalse = mui->item->getCommandMenu(30000, CommandContext::ACTION);

		pm.addSubMenu("Input", actionInputMenu);
		pm.addSubMenu("Consequence TRUE", actionCommandMenuTrue);
		pm.addSubMenu("Consequence FALSE", actionCommandMenuFalse);

		int result = pm.show();

		if (result > 0)
		{
			isInput = result < 20000;

			if (isInput)
			{
				StandardCondition * c = dynamic_cast<StandardCondition *>(action->cdm.addItem(action->cdm.factory.create(StandardCondition::getTypeStringStatic(false))));
				Controllable * target = actionInputMenu.getControllableForResult(result);
				if (c != nullptr) c->sourceTarget->setValueFromTarget(target);
			}
			else //command
			{
				isConsequenceTrue = result > 20000 && result < 30000;
				def = mui->item->getCommandDefinitionForItemID(result - 1 - (isConsequenceTrue?20000:30000));
			}
		}
	}
	else if (dataType == "CommandTemplate")
	{
		PopupMenu pm;
		pm.addItem(1, "Consequence TRUE");
		pm.addItem(2, "Consequence FALSE");

		int result = pm.show();
		if (result > 0)
		{
			isConsequenceTrue = result == 1;

			BaseItemUI<CommandTemplate> * tui = dynamic_cast<BaseItemUI<CommandTemplate> *>(details.sourceComponent.get());
			if (tui != nullptr)
			{
				CommandTemplateManager * ctm = dynamic_cast<CommandTemplateManager *>(tui->item->parentContainer.get());
				if (ctm != nullptr) def = ctm->defManager.getCommandDefinitionFor(ctm->menuName, tui->item->niceName);
			}
		}
	}

	if (!isInput && def != nullptr)
	{
		Consequence * c = isConsequenceTrue ? action->csmOn->addItem() : action->csmOff->addItem();
		if(c != nullptr) c->setCommand(def);
	}
}

void ActionUI::newMessage(const Action::ActionEvent & e)
{
	switch (e.type)
	{
	case Action::ActionEvent::ENABLED_CHANGED:
		break;

	case Action::ActionEvent::ROLE_CHANGED:
		updateBGColor();
		break;

	case Action::ActionEvent::VALIDATION_CHANGED:
		if (action->actionRoles.size() == 0) shouldRepaint = true;
		break;

	}
}

void ActionUI::addContextMenuItems(PopupMenu& p)
{
	p.addItem(100, "Copy conditions");
	p.addItem(101, "Paste conditions");

	if (action->csmOn != nullptr)
	{
		p.addItem(102, "Copy consequences TRUE");
		p.addItem(103, "Paste consequences TRUE");
	}

	if (action->csmOff != nullptr)
	{
		p.addItem(104, "Copy consequences FALSE");
		p.addItem(105, "Paste consequences FALSE");
	}
}

void ActionUI::handleContextMenuResult (int result)
{
	switch (result)
	{
		case 100: SystemClipboard::copyTextToClipboard(JSON::toString(action->cdm.getJSONData())); break;
		case 101: action->cdm.loadJSONData(JSON::fromString(SystemClipboard::getTextFromClipboard())); break;
		case 102: SystemClipboard::copyTextToClipboard(JSON::toString(action->csmOn->getJSONData())); break;
		case 103: action->csmOn->loadJSONData(JSON::fromString(SystemClipboard::getTextFromClipboard())); break;
		case 104: SystemClipboard::copyTextToClipboard(JSON::toString(action->csmOff->getJSONData())); break;
		case 105: action->csmOff->loadJSONData(JSON::fromString(SystemClipboard::getTextFromClipboard())); break;
	}
}
