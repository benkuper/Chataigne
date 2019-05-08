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
	itemDataType = "State";
	helpID = "StateMachine";

	addChildControllableContainer(&stm);
	stm.hideInEditor = true;
	stm.addBaseManagerListener(this);

	addChildControllableContainer(&commentManager);

}

StateManager::~StateManager()
{
}


void StateManager::clear()
{
	stm.clear();
	commentManager.clear();
	BaseManager::clear();
}

void StateManager::setStateActive(State * s)
{
	Array<State *> linkedStates = getLinkedStates(s);
	for (auto &ss : linkedStates)
	{
		ss->active->setValue(false);
	}
}

void StateManager::addItemInternal(State * s, var data)
{
	s->addStateListener(this);
	if (!Engine::mainEngine->isLoadingFile) s->active->setValue(true);	
}

void StateManager::removeItemInternal(State * s)
{
	s->removeStateListener(this);
}

Array<UndoableAction *> StateManager::getRemoveItemUndoableAction(State * item)
{
	Array<UndoableAction *> result;
	result.addArray(stm.getRemoveAllLinkedTransitionsAction(item));
	result.addArray(BaseManager::getRemoveItemUndoableAction(item));

	return result;
}

Array<UndoableAction*> StateManager::getRemoveItemsUndoableAction(Array<State*> itemsToRemove)
{
	Array<UndoableAction *> result;
	for (auto &i : itemsToRemove) result.addArray(stm.getRemoveAllLinkedTransitionsAction(i));
	result.addArray(BaseManager::getRemoveItemsUndoableAction(itemsToRemove));
	return result;
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


State * StateManager::showMenuAndGetState()
{
	PopupMenu menu;
	StateManager * sm = StateManager::getInstance();
	for (int i = 0; i < sm->items.size(); i++)
	{
		menu.addItem(1+i,sm->items[i]->niceName);
	}

	int result = menu.show();
	return sm->getStateForItemID(result);
}


State * StateManager::getStateForItemID(int itemID)
{
	if (itemID <= 0 || itemID > items.size()) return nullptr;
	return items[itemID-1];
}

Action * StateManager::showMenuAndGetAction()
{
	PopupMenu menu;
	StateManager * sm = StateManager::getInstance();
	for (int i = 0; i < sm->items.size(); i++)
	{
		PopupMenu sMenu;
		int numValues = sm->items[i]->pm.items.size();
		for (int j = 0; j < numValues; j++)
		{
			if (sm->items[i]->pm.items[j]->type == Processor::ACTION)
			{
				Action * c = (Action *)(sm->items[i]->pm.items[j]);
				sMenu.addItem(i * 1000 + j + 1, c->niceName);
			}
		}
		menu.addSubMenu(sm->items[i]->niceName, sMenu);
	}

	int result = menu.show();
	return sm->getActionForItemID(result);
}

Action * StateManager::getActionForItemID(int itemID)
{
	if (itemID <= 0) return nullptr;
	int stateIndex = (int)floor((itemID - 1) / 1000);
	int actionIndex = (itemID - 1) % 1000;
	return (Action *)(items[stateIndex]->pm.items[actionIndex]);
}

Mapping * StateManager::showMenuAndGetMapping()
{
	PopupMenu menu;
	StateManager * sm = StateManager::getInstance();
	for (int i = 0; i < sm->items.size(); i++)
	{
		PopupMenu sMenu;
		int numValues = sm->items[i]->pm.items.size();
		for (int j = 0; j < numValues; j++)
		{
			if (sm->items[i]->pm.items[j]->type == Processor::MAPPING)
			{
				Mapping * c = (Mapping *)(sm->items[i]->pm.items[j]);
				sMenu.addItem(i * 1000 + j + 1, c->niceName);
			}
			
		}
		menu.addSubMenu(sm->items[i]->niceName, sMenu);
	}

	int result = menu.show();
	return sm->getMappingForItemID(result);
}

Mapping * StateManager::getMappingForItemID(int itemID)
{
	if (itemID <= 0) return nullptr;
	int stateIndex = (int)floor((itemID - 1) / 1000);
	int mappingIndex = (itemID - 1) % 1000;
	return (Mapping *)(items[stateIndex]->pm.items[mappingIndex]);
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

	var tData = stm.getJSONData();
	if (!tData.isVoid() && tData.getDynamicObject()->getProperties().size() > 0) data.getDynamicObject()->setProperty("transitions",tData);
	var cData = commentManager.getJSONData();
	if (!cData.isVoid() && cData.getDynamicObject()->getProperties().size() > 0) data.getDynamicObject()->setProperty("comments", cData);
	return data;
}

void StateManager::loadJSONDataInternal(var data)
{
	BaseManager::loadJSONDataInternal(data);
	stm.loadJSONData(data.getProperty("transitions", var()));
	commentManager.loadJSONData(data.getProperty("comments", var()));
}