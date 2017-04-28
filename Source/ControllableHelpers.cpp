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
#include "InspectableEditor.h"

ControllableChooserPopupMenu::ControllableChooserPopupMenu(ControllableContainer * rootContainer, bool _showParameters, bool _showTriggers) :
	showParameters(_showParameters),
	showTriggers(_showTriggers)
{
	int id = 1;
	//if (rootContainer == nullptr) rootContainer = NodeManager::getInstance(); //to replace with global app container containing nodes, controllers, rules, etc...
	populateMenu(this, rootContainer,id);
}

ControllableChooserPopupMenu::~ControllableChooserPopupMenu()
{
}

void ControllableChooserPopupMenu::populateMenu(PopupMenu * subMenu, ControllableContainer * container, int &currentId)
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

			if (c->type == Controllable::TRIGGER)
			{
				if (!showTriggers) continue;
			} else
			{
				if (!showParameters) continue;
			}
			
			subMenu->addItem(currentId, c->niceName);
			controllableList.add(c);
			currentId++;
		}
	}

	
}

Controllable * ControllableChooserPopupMenu::showAndGetControllable()
{
	int result = show();

	if (result == 0) return nullptr;

	return controllableList[result-1];
}


int ControllableComparator::compareElements(Controllable * c1, Controllable * c2)
{
	return c1->niceName.compareIgnoreCase(c2->niceName);
}

