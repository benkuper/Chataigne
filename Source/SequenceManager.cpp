/*
  ==============================================================================

    SequenceManager.cpp
    Created: 28 Oct 2016 8:13:04pm
    Author:  bkupe

  ==============================================================================
*/

#include "SequenceManager.h"
#include "SequenceLayerFactory.h"
#include "SequenceLayerManager.h"
#include "TimeCueManager.h"

juce_ImplementSingleton(SequenceManager)

SequenceManager::SequenceManager() :
	BaseManager("Sequences"),
	module(this)
{
	helpID = "TimeMachine";
}

SequenceManager::~SequenceManager()
{
	SequenceLayerFactory::deleteInstance();
}


PopupMenu SequenceManager::getAllSequencesMenu()
{
	PopupMenu menu;
	for (int i = 0; i < items.size(); i++)
	{
		menu.addItem(1 + i, items[i]->niceName);
	}

	return menu;
}

Sequence * SequenceManager::getSequenceForItemID(int itemID)
{
	return items[itemID - 1];
}

PopupMenu SequenceManager::getAllLayersMenu()
{
	PopupMenu menu;
	for (int i = 0; i < items.size(); i++)
	{
		PopupMenu sMenu;
		int numValues = items[i]->layerManager->items.size();
		for (int j = 0; j < numValues; j++)
		{
			SequenceLayer * c = items[i]->layerManager->items[j];
			sMenu.addItem(i * 1000 + j + 1, c->niceName);
		}
		menu.addSubMenu(items[i]->niceName, sMenu);
	}

	return menu;
}

SequenceLayer * SequenceManager::getLayerForItemID(int itemID)
{
	if (itemID <= 0) return nullptr;
	int moduleIndex = (int)floor((itemID - 1) / 1000);
	int valueIndex = (itemID - 1) % 1000;
	return items[moduleIndex]->layerManager->items[valueIndex];
}

/*
PopupMenu SequenceManager::getAllCuesMenu()
{
	PopupMenu menu;
	for (int i = 0; i < items.size(); i++)
	{
		PopupMenu sMenu;
		int numValues = items[i]->cueManager->items.size();
		for (int j = 0; j < numValues; j++)
		{
			TimeCue * c = items[i]->cueManager->items[j];
			sMenu.addItem(i * 1000 + j + 1, c->niceName);
		}
		menu.addSubMenu(items[i]->niceName, sMenu);
	}
	return menu;
}

TimeCue * SequenceManager::getCueForItemID(int itemID)
{
	if (itemID <= 0) return nullptr;
	int moduleIndex = (int)floor((itemID - 1) / 1000);
	int valueIndex = (itemID - 1) % 1000;
	return items[moduleIndex]->cueManager->items[valueIndex];
}
*/