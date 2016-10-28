/*
  ==============================================================================

    ControllableHelpers.cpp
    Created: 12 May 2016 4:21:18pm
    Author:  bkupe

  ==============================================================================
*/

#include "ControllableHelpers.h"
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
	if (subMenu != this)
	{
		for (auto &c : container->controllables)
		{
			if (c->isControllableExposed)
			{
				subMenu->addItem(currentId, c->niceName);
				controllableList.add(c);
				currentId++;
			}
		}
	}

	for (auto &cc : container->controllableContainers)
	{
		PopupMenu p;
		populateMenu(&p, cc,currentId);
		subMenu->addSubMenu(cc->niceName, p);
	}
}

Controllable * ControllableContainerPopupMenu::showAndGetControllable()
{
	int result = show();

	if (result == 0) return nullptr;

	return controllableList[result-1];
}



ControllableChooser::ControllableChooser(ControllableContainer * container) :
	rootContainer(container),
	TextButton("Target")
{
	addListener(this);
	setTooltip("Choose a target");
}

ControllableChooser::~ControllableChooser()
{
	removeListener(this);
}

void ControllableChooser::setCurrentControllale(Controllable * c)
{
	if (currentControllable == c) return;
	currentControllable = c;

	if(c != nullptr)
	{
		setTooltip("Current Controllable :" + c->niceName + String("\n") + c->controlAddress);
		setButtonText(c->niceName);
	}else
	{
		setTooltip("Choose a controllable");
		setButtonText("Target");
	}

	listeners.call(&Listener::choosedControllableChanged, c);
}

void ControllableChooser::buttonClicked(Button *)
{
	ControllableContainerPopupMenu p(rootContainer);
	Controllable * c = p.showAndGetControllable();
	if(c != nullptr) setCurrentControllale(c);
}
