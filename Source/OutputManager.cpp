/*
  ==============================================================================

    OutputManager.cpp
    Created: 28 Oct 2016 8:02:49pm
    Author:  bkupe

  ==============================================================================
*/

#include "OutputManager.h"
#include "OutputFactory.h"
#include "CommandFactory.h"

juce_ImplementSingleton(OutputManager)

OutputManager::OutputManager() :
	BaseManager<Output>("Outputs")
{
}

OutputManager::~OutputManager()
{
	OutputFactory::deleteInstance();
}


void OutputManager::addItemFromData(var data)
{
	String outputType = data.getProperty("type", "none");
	if (outputType.isEmpty()) return;
	Output * o = OutputFactory::getInstance()->createOutput(outputType);
	if (o != nullptr) addItem(o, data);
}

PopupMenu OutputManager::getAllOutputsCommandMenu()
{
	PopupMenu menu;
	for (int i = 0; i < items.size(); i++) menu.addSubMenu(items[i]->niceName, items[i]->getCommandMenu(i * 1000));
	return menu;
}

CommandDefinition * OutputManager::getCommandDefinitionForItemID(int itemID)
{
	if (itemID <= 0) return nullptr;
	int outputIndex = (int)floor(itemID / 1000);
	int commandIndex = itemID % 1000 - 1;
	return items[outputIndex]->commandDefs[commandIndex];
}
