/*
  ==============================================================================

	ProcessorManagerUI.cpp
	Created: 15 Oct 2017 1:26:01pm
	Author:  Ben

  ==============================================================================
*/

#include "ProcessorManagerUI.h"
#include "Common/Command/CommandDefinition.h"
#include "Module/ui/ModuleUI.h"
#include "StateMachine/StateManager.h"
#include "Common/Processor/Action/Consequence/Consequence.h"
#include "CustomVariables/CVGroup.h"
#include "../Action/Condition/conditions/StandardCondition/StandardCondition.h"

ProcessorManagerUI::ProcessorManagerUI(ProcessorManager * _manager) :
	BaseManagerUI("Processors", _manager)
{
	addExistingItems();

	acceptedDropTypes.add("Action");
	acceptedDropTypes.add("Mapping");
	acceptedDropTypes.add("Module");
	acceptedDropTypes.add("CommandTemplate");
	//acceptedDropTypes.add("CVGroup");

	noItemText = "Add trigger interaction with an Action, or continuous interaction with a Mapping";
}

ProcessorManagerUI::~ProcessorManagerUI()
{
}

void ProcessorManagerUI::itemDropped(const SourceDetails & details)
{
	BaseManagerUI::itemDropped(details);

	String dataType = details.description.getProperty("dataType", "");
	CommandDefinition * def = nullptr;
	bool isForAction = false;
	bool isInput = false;

	if (dataType == "Module")
	{
		ModuleUI * mui = dynamic_cast<ModuleUI *>(details.sourceComponent.get());

		PopupMenu pm;

		ControllableChooserPopupMenu actionInputMenu(&mui->item->valuesCC, true, true, 0);
		ControllableChooserPopupMenu mappingInputMenu(&mui->item->valuesCC, true, false, 10000);

		PopupMenu actionCommandMenu = mui->item->getCommandMenu(20000, CommandContext::ACTION);
		PopupMenu mappingCommandMenu = mui->item->getCommandMenu(30000, CommandContext::MAPPING);

		pm.addSubMenu("Action Input", actionInputMenu);
		pm.addSubMenu("Mapping Input", mappingInputMenu);
		pm.addSubMenu("Action Consequence", actionCommandMenu);
		pm.addSubMenu("Mapping Output", mappingCommandMenu);

		int result = pm.show();

		if (result > 0)
		{
			isForAction = (result > 0 && result < 10000) || (result > 20000 && result < 30000);
			isInput = result < 20000;

			if (isInput)
			{
				if (isForAction)
				{
					Action * a = dynamic_cast<Action *>(manager->addItem(manager->factory.create("Action")));
					if (a != nullptr)
					{
						StandardCondition * c = dynamic_cast<StandardCondition *>(a->cdm.addItem(a->cdm.factory.create(StandardCondition::getTypeStringStatic())));
						Controllable * target = actionInputMenu.getControllableForResult(result);
						if (c != nullptr) c->sourceTarget->setValueFromTarget(target);
					}
				}
				else //mapping
				{
					Mapping * mapp = dynamic_cast<Mapping *>(manager->addItem(manager->factory.create("Mapping")));
					if (mapp != nullptr)
					{
						Controllable * target = mappingInputMenu.getControllableForResult(result);
						mapp->input.inputTarget->setValueFromTarget(target);
					}
				}
			}
			else //command
			{
				def = mui->item->getCommandDefinitionForItemID(result - 1 - (isForAction ? 20000 : 30000));
			}
		}
	}
	else if (dataType == "CommandTemplate")
	{
		BaseItemUI<CommandTemplate> * tui = dynamic_cast<BaseItemUI<CommandTemplate> *>(details.sourceComponent.get());
		if (tui != nullptr)
		{
			PopupMenu pm;
			pm.addItem(1, "Action Consequence");
			pm.addItem(2, "Mapping Output");

			int result = pm.show();
			if (result > 0)
			{
				isForAction = result == 1;

				CommandTemplateManager * ctm = dynamic_cast<CommandTemplateManager *>(tui->item->parentContainer.get());
				if (ctm != nullptr) def = ctm->defManager.getCommandDefinitionFor(ctm->menuName, tui->item->niceName);
			}
		}
	}

	if (!isInput && def != nullptr)
	{
		if (isForAction)
		{
			Action * a = dynamic_cast<Action *>(manager->addItem(manager->factory.create("Action")));
			if (a != nullptr)
			{
				Consequence * c = a->csmOn->addItem();
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
			Mapping * mapp = dynamic_cast<Mapping *>(manager->addItem(manager->factory.create("Mapping")));
			if (mapp != nullptr)
			{
				MappingOutput * o = mapp->om.addItem();
				o->setCommand(def);

				if (manager->items.size() > 0)
				{
					if (currentDropIndex == -1) currentDropIndex = manager->items.size() - 1;
					manager->setItemIndex(mapp, currentDropIndex);
				}
			}
		}
	}

}

ProcessorUI * ProcessorManagerUI::createUIForItem(Processor *p)
{
	return p->getUI();
}
