/*
  ==============================================================================

    ConditionFactory.cpp
    Created: 21 Feb 2017 11:56:12am
    Author:  Ben

  ==============================================================================
*/

#include "ConditionFactory.h"

#include "StandardCondition.h"
#include "ConditionGroup.h"
#include "ScriptCondition.h"

juce_ImplementSingleton(ConditionFactory)

ConditionFactory::ConditionFactory() {
	conditionDefs.add(new ConditionDefinition("", "Standard", &StandardCondition::create));
	conditionDefs.add(new ConditionDefinition("", "Group", &ConditionGroup::create));
	conditionDefs.add(new ConditionDefinition("", "Script", &ScriptCondition::create));

	buildPopupMenu();
}

void ConditionFactory::buildPopupMenu()
{
	OwnedArray<PopupMenu> subMenus;
	Array<String> subMenuNames;

	for (auto &d : conditionDefs)
	{
		int itemID = conditionDefs.indexOf(d) + 1;//start at 1 for menu

		if (d->menuPath.isEmpty())
		{
			menu.addItem(itemID, d->conditionType);
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

		subMenus[subMenuIndex]->addItem(itemID, d->conditionType);
	}

	for (int i = 0; i < subMenus.size(); i++) menu.addSubMenu(subMenuNames[i], *subMenus[i]);
}
