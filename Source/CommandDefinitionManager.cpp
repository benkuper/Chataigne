/*
  ==============================================================================

    CommandDefinitionManager.cpp
    Created: 19 Dec 2016 3:48:25pm
    Author:  Ben

  ==============================================================================
*/

#include "CommandDefinitionManager.h"


CommandDefinitionManager::CommandDefinitionManager() :
	BaseManager("Definitions")
{
	recursiveInspectionLevel = 1;
}

CommandDefinitionManager::~CommandDefinitionManager()
{
}

void CommandDefinitionManager::rebuildCommandMenu(int baseID)
{
	commandMenu.clear();
	OwnedArray<PopupMenu> subMenus;
	Array<String> subMenuNames;

	for (auto &d : items)
	{
		int itemID = items.indexOf(d) + 1 + baseID;//start at baseID + 1 for menu
		if (d->menuPath.isEmpty())
		{
			commandMenu.addItem(itemID, d->commandType);
			continue;
		}

		int subMenuIndex = -1;
		for (int i = 0; i < subMenus.size(); i++)
		{
			if (subMenuNames[i] == d->menuPath)
			{
				subMenuIndex = i;
				break;
			}
		}
		if (subMenuIndex == -1)
		{
			subMenuNames.add(d->menuPath);
			subMenus.add(new PopupMenu());
			subMenuIndex = subMenus.size() - 1;
		}


		subMenus[subMenuIndex]->addItem(itemID, d->commandType);
	}

	for (int i = 0; i < subMenus.size(); i++) commandMenu.addSubMenu(subMenuNames[i], *subMenus[i]);
}

PopupMenu CommandDefinitionManager::getCommandMenu(int baseID)
{
	rebuildCommandMenu(baseID);
	return commandMenu;
}

CommandDefinition * CommandDefinitionManager::getCommandDefinitionFor(const String & menuPath, const String & moduleType)
{
	for (auto &d : items) if (d->menuPath == menuPath && d->commandType == moduleType) return d;
	return nullptr;
}

