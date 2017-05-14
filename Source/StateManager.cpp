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
module(this),
stm(this)
{
	isSelectable = true;
	addChildControllableContainer(&stm);
	stm.hideInEditor = true;
	stm.addBaseManagerListener(this);

	onlyOneActiveState = addBoolParameter("One Active State", "If checked, when activating a state, all other non-permanent states will be automatically deactivated.", false);
}

StateManager::~StateManager()
{
}


void StateManager::setStateActive(State * s)
{
	if (onlyOneActiveState->boolValue())
	{
		for (auto &ss : items)
		{
			ss->active->setValue(false);
		}
	} else
	{
		Array<State *> linkedStates = getLinkedStates(s);
		for (auto &ss : linkedStates)
		{
			ss->active->setValue(false);
		}
	}
}

void StateManager::addItemInternal(State * s, var data)
{
	s->addStateListener(this);
	if (!Engine::mainEngine->isLoadingFile) s->active->setValue(true);	
}

void StateManager::removeItemInternal(State * s)
{
	//DBG("StateManager removeItemInternal");
	s->removeStateListener(this);
	stm.removeAllLinkedTransitions(s);
}

void StateManager::stateActivationChanged(State * s)
{
	if (s->active->boolValue())
	{
		setStateActive(s);
	}
}

void StateManager::itemAdded(StateTransition * s)
{
	if (!Engine::mainEngine->isLoadingFile) setStateActive(s->sourceState);
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

Array<State *> StateManager::getLinkedStates(State * s, Array<State *> * statesToAvoid)
{
	Array<State *> sAvoid;
	if (statesToAvoid == nullptr)
	{
		statesToAvoid = &sAvoid;
	}

	statesToAvoid->add(s);

	Array<State *> result;

	Array<State *> linkStates = stm.getAllStatesLinkedTo(s);
	for (auto &ss : linkStates)
	{
		if (statesToAvoid->contains(ss)) continue;
		result.add(ss);
		statesToAvoid->add(ss);
		Array<State *> linkedSS = getLinkedStates(ss, statesToAvoid);
		result.addArray(linkedSS);
	}

	return result;
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