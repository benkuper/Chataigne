/*
  ==============================================================================

    Output.cpp
    Created: 28 Oct 2016 8:02:31pm
    Author:  bkupe

  ==============================================================================
*/

#include "Output.h"
#include "CommandFactory.h"

Output::Output(const String &name) :
	BaseItem(name)
{
}

Output::~Output()
{
}

void Output::rebuildCommandMenu(int baseID)
{
	commandMenu.clear();
	OwnedArray<PopupMenu> subMenus;
	Array<String> subMenuNames;

	for (auto &d : commandDefs)
	{
		int itemID = commandDefs.indexOf(d) + 1 + baseID;//start at baseID + 1 for menu
		if (d->menuPath.isEmpty())
		{
			commandMenu.addItem(itemID, d->inputType);
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

		
		subMenus[subMenuIndex]->addItem(itemID, d->inputType);
	}

	for (int i = 0; i < subMenus.size(); i++) commandMenu.addSubMenu(subMenuNames[i], *subMenus[i]);
}

PopupMenu Output::getCommandMenu(int baseID)
{
	rebuildCommandMenu(baseID);
	return commandMenu;
}
