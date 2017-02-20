/*
  ==============================================================================

    StateManager.cpp
    Created: 28 Oct 2016 8:19:15pm
    Author:  bkupe

  ==============================================================================
*/

#include "StateManager.h"

juce_ImplementSingleton(StateManager)

StateManager::StateManager() :
	BaseManager<State>("States"),
	activeState(nullptr),
	stm(this),
	module(this)
{
	isSelectable = true;
	addChildControllableContainer(&stm);
}

StateManager::~StateManager()
{
}

void StateManager::setActiveState(State * s)
{
	if (s == activeState) return;

	if (!activeState.wasObjectDeleted() && activeState != nullptr)
	{
		activeState->active->setValue(false);
	}

	activeState = s;
}



State * StateManager::addItem(const Point<float>& initialPosition)
{
	State * s = BaseManager::addItem();
	s->viewUIPosition->setPoint(initialPosition);
	return s;
}

void StateManager::addItemInternal(State * s, var data)
{
	s->addStateListener(this);
	if (s->active->boolValue()) setActiveState(s);
	else if (activeState == nullptr) s->active->setValue(true);
}

void StateManager::removeItemInternal(State * s)
{
	DBG("StateManager removeItemInternal");
	s->removeStateListener(this);
	stm.removeAllLinkedTransitions(s);
}

void StateManager::stateActivationChanged(State * s)
{
	if (s->permanent->boolValue()) return; //don't care of permanent in state logic.
	if (s->active->boolValue()) setActiveState(s);
}

PopupMenu StateManager::getAllStatesMenu()
{
	PopupMenu menu;
	for (int i = 0; i < items.size(); i++)
	{
		menu.addItem(1+i,items[i]->niceName);
	}

	return menu;
}

State * StateManager::getStateForItemID(int itemID)
{
	return items[itemID-1];
}

PopupMenu StateManager::getAllActionsMenu()
{
	PopupMenu menu;
	for (int i = 0; i < items.size(); i++)
	{
		PopupMenu sMenu;
		int numValues = items[i]->am.items.size();
		for (int j = 0; j < numValues; j++)
		{
			Action * c = items[i]->am.items[j];
			sMenu.addItem(i * 1000 + j + 1, c->niceName);
		}
		menu.addSubMenu(items[i]->niceName, sMenu);
	}

	return menu;
}

Action * StateManager::getActionForItemID(int itemID)
{
	if (itemID <= 0) return nullptr;
	int moduleIndex = (int)floor((itemID - 1) / 1000);
	int valueIndex = (itemID - 1) % 1000;
	return items[moduleIndex]->am.items[valueIndex];
}

PopupMenu StateManager::getAllMappingsMenu()
{
	PopupMenu menu;
	for (int i = 0; i < items.size(); i++)
	{
		PopupMenu sMenu;
		int numValues = items[i]->mm.items.size();
		for (int j = 0; j < numValues; j++)
		{
			Mapping * c = items[i]->mm.items[j];
			sMenu.addItem(i * 1000 + j + 1, c->niceName);
		}
		menu.addSubMenu(items[i]->niceName, sMenu);
	}

	return menu;
}

Mapping * StateManager::getMappingForItemID(int itemID)
{
	if (itemID <= 0) return nullptr;
	int moduleIndex = (int)floor((itemID - 1) / 1000);
	int valueIndex = (itemID - 1) % 1000;
	return items[moduleIndex]->mm.items[valueIndex];
}

var StateManager::getJSONData()
{
	var data = BaseManager::getJSONData();
	data.getDynamicObject()->setProperty("transitions", stm.getJSONData());
	return data;
}

void StateManager::loadJSONDataInternal(var data)
{
	BaseManager::loadJSONDataInternal(data);
	stm.loadJSONData(data.getProperty("transitions", var()));
}

InspectableEditor * StateManager::getEditor(bool /*isRoot*/)
{
	return nullptr;
}
