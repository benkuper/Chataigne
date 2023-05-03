/*
  ==============================================================================

	ProcessorManagerUI.cpp
	Created: 15 Oct 2017 1:26:01pm
	Author:  Ben

  ==============================================================================
*/

#include "Common/Command/CommandIncludes.h"

#include "Module/ModuleIncludes.h"
#include "StateMachine/StateMachineIncludes.h"
#include "CustomVariables/CustomVariablesIncludes.h"

ProcessorManagerUI::ProcessorManagerUI(ProcessorManager* _manager, bool useViewport) :
	BaseManagerUI("Processors", _manager, useViewport)
{
	drawContour = false;

	setShowSearchBar(true);

	acceptedDropTypes.add("Action");
	acceptedDropTypes.add("Mapping");
	acceptedDropTypes.add("Module");
	acceptedDropTypes.add("CommandTemplate");
	//acceptedDropTypes.add("CVGroup"); 

	noItemText = "Add trigger interaction with an Action, or continuous interaction with a Mapping";

	addExistingItems();
}

ProcessorManagerUI::~ProcessorManagerUI()
{
}

void ProcessorManagerUI::itemDropped(const SourceDetails& details)
{
	BaseManagerUI::itemDropped(details);

	String dataType = details.description.getProperty("dataType", "");


	std::function<void(CommandDefinition*, bool, bool)> createDef = [this](CommandDefinition* def, bool isInput, bool isForAction)
	{
		if (!isInput && def != nullptr)
		{
			if (isForAction)
			{
				Action* a = dynamic_cast<Action*>(manager->addItem(manager->factory.create("Action")));
				if (a != nullptr)
				{
					Consequence* c = new Consequence(var(), a->multiplex);
					a->csmOn->addItem(c);
					c->setCommand(def);

					if (manager->items.size() > 0)
					{
						if (currentDropIndex == -1) currentDropIndex = manager->items.size() - 1;
						manager->setItemIndex(a, currentDropIndex);
					}
				}
			}
			else //mapping
			{
				Mapping* mapp = dynamic_cast<Mapping*>(manager->addItem(manager->factory.create("Mapping")));
				if (mapp != nullptr)
				{
					MappingOutput* o = mapp->om.addItem();
					o->setCommand(def);

					if (manager->items.size() > 0)
					{
						if (currentDropIndex == -1) currentDropIndex = manager->items.size() - 1;
						manager->setItemIndex(mapp, currentDropIndex);
					}
				}
			}
		}
	};

	if (dataType == "Module")
	{
		ModuleUI* mui = dynamic_cast<ModuleUI*>(details.sourceComponent.get());

		PopupMenu pm;

		ControllableChooserPopupMenu* actionInputMenu = new ControllableChooserPopupMenu(&mui->item->valuesCC, 0);
		ControllableChooserPopupMenu* mappingInputMenu = new ControllableChooserPopupMenu(&mui->item->valuesCC, 10000, -1, StringArray(), StringArray(Trigger::getTypeStringStatic()));

		PopupMenu actionCommandMenu = mui->item->getCommandMenu(20000, CommandContext::ACTION);
		PopupMenu mappingCommandMenu = mui->item->getCommandMenu(30000, CommandContext::MAPPING);

		pm.addSubMenu("Action Input", *actionInputMenu);
		pm.addSubMenu("Mapping Input", *mappingInputMenu);
		pm.addSubMenu("Action Consequence", actionCommandMenu);
		pm.addSubMenu("Mapping Output", mappingCommandMenu);

		pm.showMenuAsync(PopupMenu::Options(), [this, mui, actionInputMenu, mappingInputMenu, createDef](int result)
			{
				CommandDefinition* def = nullptr;
				bool isForAction = (result > 0 && result < 10000) || (result > 20000 && result < 30000);
				bool isInput = result < 20000;

				if (isInput)
				{
					if (isForAction)
					{
						Action* a = dynamic_cast<Action*>(manager->addItem(manager->factory.create("Action")));
						if (a != nullptr && a->cdm != nullptr)
						{
							StandardCondition* c = dynamic_cast<StandardCondition*>(a->cdm->addItem(a->cdm->factory.create(StandardCondition::getTypeStringStatic(false))));
							Controllable* target = actionInputMenu->getControllableForResult(result);
							if (c != nullptr) c->sourceTarget->setValueFromTarget(target);
						}
					}
					else //mapping
					{
						Mapping* mapp = dynamic_cast<Mapping*>(manager->addItem(manager->factory.create("Mapping")));
						if (mapp != nullptr)
						{
							Controllable* target = mappingInputMenu->getControllableForResult(result);
							StandardMappingInput* mi = new StandardMappingInput();
							mapp->im.addItem(mi);
							mi->inputTarget->setValueFromTarget(target);
						}
					}
				}
				else //command
				{
					def = mui->item->getCommandDefinitionForItemID(result - 1 - (isForAction ? 20000 : 30000));
				}

				createDef(def, isInput, isForAction);

				delete actionInputMenu;
				delete mappingInputMenu;
			}
		);
	}
	else if (dataType == "CommandTemplate")
	{
		BaseItemUI<CommandTemplate>* tui = dynamic_cast<BaseItemUI<CommandTemplate> *>(details.sourceComponent.get());
		if (tui != nullptr)
		{
			PopupMenu pm;
			pm.addItem(1, "Action Consequence");
			pm.addItem(2, "Mapping Output");

			pm.showMenuAsync(PopupMenu::Options(), [tui, createDef](int result)
				{
					if (result == 0) return;

					bool isForAction = result == 1;

					CommandTemplateManager* ctm = dynamic_cast<CommandTemplateManager*>(tui->item->parentContainer.get());
					if (ctm != nullptr)
					{
						CommandDefinition* def = ctm->defManager->getCommandDefinitionFor(ctm->menuName, tui->item->niceName);
						createDef(def, false, isForAction);
					}

				}
			);
		}
	}



}

void ProcessorManagerUI::addItemFromMenu(Processor* item, bool isFromAddButton, Point<int> mouseDownPos)
{
	BaseManagerUI::addItemFromMenu(item, isFromAddButton, mouseDownPos);

	if (Action* a = dynamic_cast<Action*>(item))
	{
		if (a->cdm != nullptr) a->cdm->addItem(a->cdm->factory.create(StandardCondition::getTypeStringStatic(a->isMultiplexed()))); //add one condition by default if done through UI
	}
	else if (Mapping* m = dynamic_cast<Mapping*>(item))
	{
		m->im.addItem(m->im.factory.create(StandardMappingInput::getTypeStringStatic(m->isMultiplexed()))); //add one input by default if done through UI
	}
}

ProcessorUI* ProcessorManagerUI::createUIForItem(Processor* p)
{
	return p->getUI();
}
