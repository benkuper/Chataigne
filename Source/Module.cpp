/*
  ==============================================================================

    Module.cpp
    Created: 8 Dec 2016 2:36:02pm
    Author:  Ben

  ==============================================================================
*/

#include "Module.h"
#include "CommandFactory.h"

Module::Module(const String &name) :
	BaseItem(name),
	valuesCC("Values")
{
	saveAndLoadRecursiveData = false; //manage value ourselves

	recursiveInspectionLevel = 1;
	addChildControllableContainer(&valuesCC);

	logIncomingData = addBoolParameter("Log Incoming Data", "Enable / Disable logging of incoming data for this module", false);
	logIncomingData->hideInOutliner = true;
	logIncomingData->isTargettable = false;

	activityTrigger = addTrigger("Activity", "Activity Signal");
	activityTrigger->hideInEditor = true;
}

Module::~Module()
{

}

Array<WeakReference<Controllable>> Module::getValueControllables()
{
	return valuesCC.getAllControllables();
}


void Module::rebuildCommandMenu(int baseID)
{
	commandMenu.clear();
	OwnedArray<PopupMenu> subMenus;
	Array<String> subMenuNames;

	for (auto &d : commandDefs)
	{
		int itemID = commandDefs.indexOf(d) + 1 + baseID;//start at baseID + 1 for menu
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

PopupMenu Module::getCommandMenu(int baseID)
{
	rebuildCommandMenu(baseID);
	return commandMenu;
}

CommandDefinition * Module::getCommandDefinitionFor(const String & menuPath, const String & moduleType)
{
	for (auto &d : commandDefs) if (d->menuPath == menuPath && d->commandType == moduleType) return d;
	return nullptr;
}


var Module::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("type", getTypeString());
	return data;
}
