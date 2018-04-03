/*
  ==============================================================================

    SequenceManager.cpp
    Created: 28 Oct 2016 8:13:04pm
    Author:  bkupe

  ==============================================================================
*/

#include "SequenceManager.h"
#include "Sequence/Layer/SequenceLayerFactory.h"
#include "Sequence/Layer/SequenceLayerManager.h"
#include "Sequence/Cue/TimeCueManager.h"

juce_ImplementSingleton(SequenceManager)

SequenceManager::SequenceManager() :
	BaseManager("Sequences"),
	module(this)
{
	itemDataType = "Sequence";
	helpID = "TimeMachine";
}

SequenceManager::~SequenceManager()
{
	SequenceLayerFactory::deleteInstance();
}


Sequence * SequenceManager::showMenuAndGetSequence()
{
	PopupMenu menu;
	int numItems = SequenceManager::getInstance()->items.size();
	for (int i = 0; i < numItems; i++)
	{
		menu.addItem(1 + i, SequenceManager::getInstance()->items[i]->niceName);
	}
	int result = menu.show();
	return SequenceManager::getInstance()->getSequenceForItemID(result);
}

Sequence * SequenceManager::getSequenceForItemID(int itemID)
{
	if (itemID <= 0 || itemID > items.size()) return nullptr;
	return items[itemID - 1];
}

SequenceLayer * SequenceManager::showmMenuAndGetLayer()
{
	PopupMenu menu;
	SequenceManager * sm = SequenceManager::getInstance();
	for (int i = 0; i < sm->items.size(); i++)
	{
		PopupMenu sMenu;
		int numValues = sm->items[i]->layerManager->items.size();
		for (int j = 0; j < numValues; j++)
		{
			SequenceLayer * c = sm->items[i]->layerManager->items[j];
			sMenu.addItem(i * 1000 + j + 1, c->niceName);
		}
		menu.addSubMenu(sm->items[i]->niceName, sMenu);
	}

	int result = menu.show();
	if (result == 0) return nullptr;
	return SequenceManager::getInstance()->getLayerForItemID(result);
}

SequenceLayer * SequenceManager::getLayerForItemID(int itemID)
{
	if (itemID <= 0) return nullptr;
	int sequenceIndex = (int)floor((itemID - 1) / 1000);
	int layerIndex = (itemID - 1) % 1000;
	return items[sequenceIndex]->layerManager->items[layerIndex];
}

TimeCue * SequenceManager::showMenuAndGetCue()
{
	PopupMenu menu;
	SequenceManager * sm = SequenceManager::getInstance();
	for (int i = 0; i < sm->items.size(); i++)
	{
		PopupMenu sMenu;
		int numValues = sm->items[i]->cueManager->items.size();
		for (int j = 0; j < numValues; j++)
		{
			TimeCue * c = sm->items[i]->cueManager->items[j];
			sMenu.addItem(i * 1000 + j + 1, c->niceName);
		}
		menu.addSubMenu(sm->items[i]->niceName, sMenu);
	}

	int result = menu.show();
	return SequenceManager::getInstance()->getCueForItemID(result);
}

TimeCue * SequenceManager::getCueForItemID(int itemID)
{
	if (itemID <= 0) return nullptr;
	int moduleIndex = (int)floor((itemID - 1) / 1000);
	int valueIndex = (itemID - 1) % 1000;
	return items[moduleIndex]->cueManager->items[valueIndex];
}