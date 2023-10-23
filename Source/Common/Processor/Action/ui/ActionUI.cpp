/*
  ==============================================================================

	ActionUI.cpp
	Created: 28 Oct 2016 8:05:24pm
	Author:  bkupe

  ==============================================================================
*/

#include "Common/Processor/ProcessorIncludes.h"

juce_ImplementSingleton(ActionUITimers)

ActionUI::ActionUI(Action* _action, bool showMiniModeBT) :
	ProcessorUI(_action, showMiniModeBT),
	action(_action)
{
	ActionUITimers::getInstance()->registerAction(this);

	acceptedDropTypes.add("Module");
	acceptedDropTypes.add("CommandTemplate");

	action->addAsyncActionListener(this);

	triggerUI.reset(action->isMultiplexed() ? action->triggerPreview->createButtonUI() : action->triggerOn->createButtonUI());
	if (action->isMultiplexed()) triggerUI->customLabel = "Trigger (" + String(action->getPreviewIndex() + 1) + ")";

	addAndMakeVisible(triggerUI.get());

	if (action->cdm != nullptr && action->cdm->validationProgressFeedback != nullptr)
	{
		progressionUI.reset(action->cdm->validationProgressFeedback->createSlider());
		progressionUI->showValue = false;
		addChildComponent(progressionUI.get());
		progressionUI->setVisible(action->cdm->validationTime->floatValue() > 0);
	}

	if (action->csmOn != nullptr && action->csmOn->staggerProgression != nullptr)
	{
		action->csmOn->addAsyncConsequenceManagerListener(this);
		staggerUI.reset(action->csmOn->staggerProgression->createSlider());
		staggerUI->customFGColor = Colours::lightpink;
		staggerUI->useCustomFGColor = true;
		staggerUI->showValue = false;
		addChildComponent(staggerUI.get());
		updateStaggerUI();
	}
}

ActionUI::~ActionUI()
{
	if (!inspectable.wasObjectDeleted())
	{
		action->removeAsyncActionListener(this);
		if (action->csmOn)
		{
			action->csmOn->removeAsyncConsequenceManagerListener(this);
		}
	}

	if (ActionUITimers* t = ActionUITimers::getInstanceWithoutCreating()) t->unregisterAction(this);
}

void ActionUI::paint(Graphics& g)
{
	BaseItemUI::paint(g);

	if (isDraggingOver)
	{
		g.setColour(BLUE_COLOR.darker());
		g.fillRoundedRectangle(getLocalBounds().toFloat(), 2);
	}
}


void ActionUI::controllableFeedbackUpdateInternal(Controllable* c)
{
	ProcessorUI::controllableFeedbackUpdateInternal(c);
	if (action->csmOn != nullptr && c == action->csmOn->stagger && staggerUI != nullptr)
	{
		updateStaggerUI();
	}

	if (action->cdm != nullptr && c == action->cdm->validationTime && progressionUI != nullptr)
	{
		bool v = action->cdm->validationTime->floatValue() > 0;
		if (progressionUI->isVisible() != v)
		{
			progressionUI->setVisible(v);
			resized();
		}
	}
}

void ActionUI::updateStaggerUI()
{
	bool v = action->csmOn->stagger->floatValue() > 0 && action->csmOn->items.size() > 1;
	if (staggerUI->isVisible() != v)
	{
		staggerUI->setVisible(v);
		resized();
	}
}

void ActionUI::resizedInternalHeader(Rectangle<int>& r)
{
	ProcessorUI::resizedInternalHeader(r);

	if (triggerUI != nullptr) triggerUI->setBounds(r.removeFromRight(70));

	if (progressionUI != nullptr && progressionUI->isVisible())
	{
		progressionUI->setBounds(r.removeFromRight(40).reduced(2, 6));
	}

	if (staggerUI != nullptr && staggerUI->isVisible())
	{
		staggerUI->setBounds(r.removeFromRight(40).reduced(2, 6));
	}
}

void ActionUI::paintOverChildren(Graphics& g)
{
	BaseItemUI::paintOverChildren(g);
	if (action->cdm != nullptr && action->cdm->getIsValid(action->getPreviewIndex()) && action->actionRoles.size() == 0) //no special roles like activate or deactivate
	{
		g.setColour(GREEN_COLOR);
		g.drawRoundedRectangle(getMainBounds().toFloat(), rounderCornerSize, 2);
	}
}

void ActionUI::itemDropped(const SourceDetails& details)
{
	BaseItemUI::itemDropped(details);

	String dataType = details.description.getProperty("dataType", "");


	std::function<void(CommandDefinition* def, bool, bool)> createFunc = [this](CommandDefinition* def, bool isInput, bool isConsequenceTrue)
		{
			if (!isInput && def != nullptr)
			{
				Consequence* c = new Consequence(var(), action->multiplex);
				if (isConsequenceTrue) action->csmOn->addItem(c);
				else action->csmOff->addItem(c);
				c->setCommand(def);
			}
		};

	if (dataType == "Module")
	{
		ModuleUI* mui = dynamic_cast<ModuleUI*>(details.sourceComponent.get());

		PopupMenu pm;
		ControllableChooserPopupMenu actionInputMenu(&mui->item->valuesCC, 0);

		PopupMenu actionCommandMenuTrue = mui->item->getCommandMenu(20000, CommandContext::ACTION);
		PopupMenu actionCommandMenuFalse = mui->item->getCommandMenu(30000, CommandContext::ACTION);

		if (action->cdm != nullptr) pm.addSubMenu("Input", actionInputMenu);
		pm.addSubMenu("Consequence TRUE", actionCommandMenuTrue);
		pm.addSubMenu("Consequence FALSE", actionCommandMenuFalse);

		pm.showMenuAsync(PopupMenu::Options(), [this, mui, createFunc, &actionInputMenu](int result)
			{
				if (result > 0)
				{
					bool isInput = result < 20000;

					if (isInput)
					{
						if (action->cdm != nullptr)
						{
							StandardCondition* c = dynamic_cast<StandardCondition*>(action->cdm->addItem(action->cdm->factory.create(StandardCondition::getTypeStringStatic(false))));
							Controllable* target = actionInputMenu.getControllableForResult(result);
							if (c != nullptr) c->sourceTarget->setValueFromTarget(target);
						}
					}
					else //command
					{
						bool isConsequenceTrue = result > 20000 && result < 30000;
						CommandDefinition* def = mui->item->getCommandDefinitionForItemID(result - 1 - (isConsequenceTrue ? 20000 : 30000));
						createFunc(def, isInput, isConsequenceTrue);
					}
				}
			}
		);
	}
	else if (dataType == "CommandTemplate")
	{
		PopupMenu pm;
		pm.addItem(1, "Consequence TRUE");
		pm.addItem(2, "Consequence FALSE");

		pm.showMenuAsync(PopupMenu::Options(), [this, details, createFunc](int result)
			{

				if (result > 0)
				{
					bool isConsequenceTrue = result == 1;

					BaseItemUI<CommandTemplate>* tui = dynamic_cast<BaseItemUI<CommandTemplate> *>(details.sourceComponent.get());
					if (tui != nullptr)
					{
						CommandTemplateManager* ctm = dynamic_cast<CommandTemplateManager*>(tui->item->parentContainer.get());
						if (ctm != nullptr)
						{
							CommandDefinition* def = ctm->defManager->getCommandDefinitionFor(ctm->menuName, tui->item->niceName);
							createFunc(def, false, isConsequenceTrue);
						}
					}
				}
			}
		);
	}


}

void ActionUI::newMessage(const Action::ActionEvent& e)
{
	switch (e.type)
	{
	case Action::ActionEvent::ENABLED_CHANGED:
		break;

	case Action::ActionEvent::ROLE_CHANGED:
		//updateBGColor();
		break;

	case Action::ActionEvent::VALIDATION_CHANGED:
		if (action->actionRoles.size() == 0)
		{
			shouldRepaint = true;
		}
		if (action->cdm->getIsValid(action->getPreviewIndex()) && action->actionRoles.size() == 0)
		{
			processorUIListeners.call(&ProcessorUIListener::processorAskForFocus, this);
		}
		break;

	case Action::ActionEvent::MULTIPLEX_PREVIEW_CHANGED:
		if (action->isMultiplexed())
		{
			triggerUI->customLabel = "Trigger (" + String(action->getPreviewIndex() + 1) + ")";
			triggerUI->repaint();

			repaint();
		}
		break;

	}
}

void ActionUI::newMessage(const ConsequenceManager::ConsequenceManagerEvent& e)
{
	if (e.type == ConsequenceManager::ConsequenceManagerEvent::STAGGER_CHANGED)
	{
		updateStaggerUI();
	}
}

void ActionUI::handlePaintTimer()
{
	if (shouldRepaint) repaint();
	shouldRepaint = false;
}

void ActionUI::addContextMenuItems(PopupMenu& p)
{
	if (action->cdm != nullptr)
	{
		p.addItem(100, "Copy conditions");
		p.addItem(101, "Paste conditions");
	}


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

void ActionUI::handleContextMenuResult(int result)
{

	switch (result)
	{
	case 100: SystemClipboard::copyTextToClipboard(JSON::toString(action->cdm->getJSONData())); break;
	case 101: action->cdm->loadJSONData(JSON::fromString(SystemClipboard::getTextFromClipboard())); break;
	case 102: SystemClipboard::copyTextToClipboard(JSON::toString(action->csmOn->getJSONData())); break;
	case 103: action->csmOn->loadJSONData(JSON::fromString(SystemClipboard::getTextFromClipboard())); break;
	case 104: SystemClipboard::copyTextToClipboard(JSON::toString(action->csmOff->getJSONData())); break;
	case 105: action->csmOff->loadJSONData(JSON::fromString(SystemClipboard::getTextFromClipboard())); break;
	}
}

ActionUITimers::ActionUITimers()
{
	startTimerHz(20);
}

void ActionUITimers::registerAction(ActionUI* ui)
{
	actionsUI.addIfNotAlreadyThere(ui);
}

void ActionUITimers::unregisterAction(ActionUI* ui)
{
	actionsUI.removeAllInstancesOf(ui);
}

void ActionUITimers::timerCallback()
{
	for (auto& ui : actionsUI) ui->handlePaintTimer();
}