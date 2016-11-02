/*
  ==============================================================================

    ControllableHelpers.cpp
    Created: 12 May 2016 4:21:18pm
    Author:  bkupe

  ==============================================================================
*/

#include "ControllableHelpers.h"
#include "ControllableUI.h"
#include "DebugHelpers.h"
#include "ControllableContainer.h"

ControllableContainerPopupMenu::ControllableContainerPopupMenu(ControllableContainer * rootContainer)
{
	int id = 1;
	//if (rootContainer == nullptr) rootContainer = NodeManager::getInstance(); //to replace with global app container containing nodes, controllers, rules, etc...
	populateMenu(this, rootContainer,id);
}

ControllableContainerPopupMenu::~ControllableContainerPopupMenu()
{
}

void ControllableContainerPopupMenu::populateMenu(PopupMenu * subMenu, ControllableContainer * container, int &currentId)
{
	for (auto &cc : container->controllableContainers)
	{

		if (!cc->isTargettable) continue;

		PopupMenu p;
		populateMenu(&p, cc, currentId);
		subMenu->addSubMenu(cc->niceName, p);
	}

	if (subMenu != this)
	{
		for (auto &c : container->controllables)
		{
			if (!c->isTargettable || !c->isControllableExposed) continue;

			subMenu->addItem(currentId, c->niceName);
			controllableList.add(c);
			currentId++;
		}
	}

	
}

Controllable * ControllableContainerPopupMenu::showAndGetControllable()
{
	int result = show();

	if (result == 0) return nullptr;

	return controllableList[result-1];
}

int ControllableUIComparator::compareElements(ControllableUI * c1, ControllableUI * c2)
{
	return c1->controllable->niceName.compareIgnoreCase(c2->controllable->niceName);
}

int ControllableComparator::compareElements(Controllable * c1, Controllable * c2)
{
	return c1->niceName.compareIgnoreCase(c2->niceName);
}
