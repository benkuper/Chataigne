/*
  ==============================================================================

	CommandTemplateManager.cpp
	Created: 31 May 2018 11:29:27am
	Author:  Ben

  ==============================================================================
*/

#include "Common/CommonIncludes.h"

CommandTemplateManager::CommandTemplateManager(Module* module) :
	BaseManager("Templates"),
	module(module)
{
	defManager.reset(new CommandDefinitionManager());

	managerFactory = &factory;
	selectItemWhenCreated = false;
}

CommandTemplateManager::~CommandTemplateManager()
{
}

void CommandTemplateManager::addItemInternal(CommandTemplate* item, var)
{
	defManager->add(CommandDefinition::createDef(module, menuName, item->niceName, &BaseCommand::create)->addParam("template", item->shortName), items.indexOf(item));
	item->addCommandTemplateListener(this);
}

void CommandTemplateManager::removeItemInternal(CommandTemplate* item)
{
	CommandDefinition* d = defManager->getCommandDefinitionFor(menuName, item->niceName);
	defManager->remove(d);
	item->removeCommandTemplateListener(this);
}

CommandTemplate* CommandTemplateManager::addItemFromData(var data, bool addToUndo)
{
	CommandTemplate* ct = new CommandTemplate(module, data);
	return addItem(ct, data, addToUndo);
}

Array<CommandTemplate*> CommandTemplateManager::addItemsFromData(var data, bool addToUndo)
{
	Array<CommandTemplate*> itemsToAdd;
	for (int i = 0; i < data.size(); i++) itemsToAdd.add(new CommandTemplate(module, data[i]));
	return addItems(itemsToAdd, data, addToUndo);
}

void CommandTemplateManager::setupDefinitionsFromModule()
{
	factory.defs.clear();
	for (auto& d : module->defManager->definitions)
	{
		factory.defs.add(Factory<CommandTemplate>::Definition::createDef(d->menuPath, d->commandType, CommandTemplate::create)
			->addParam("module", module->shortName)
			->addParam("menuPath", d->menuPath)->addParam("commandType", d->commandType));
	}
}

void CommandTemplateManager::setItemIndex(CommandTemplate* t, int newIndex, bool addToUndo)
{
	BaseManager::setItemIndex(t, newIndex, true);
	if (!addToUndo) reorderDefinitions();
}

void CommandTemplateManager::reorderItems()
{
	BaseManager::reorderItems();
	reorderDefinitions();
}

void CommandTemplateManager::reorderDefinitions()
{
	int index = 0;
	for (auto& item : items)
	{
		CommandDefinition* d = defManager->getCommandDefinitionFor(menuName, item->niceName);
		if (d != nullptr) defManager->definitions.move(defManager->definitions.indexOf(d), index++);
	}
}


void CommandTemplateManager::templateNameChanged(CommandTemplate* ct)
{
	if (isCurrentlyLoadingData || Engine::mainEngine->isLoadingFile || Engine::mainEngine->isClearing) return;

	CommandDefinition* def = defManager->definitions[items.indexOf(ct)];
	jassert(def != nullptr);

	def->commandType = ct->niceName;
	def->params.getDynamicObject()->setProperty("template", ct->shortName);
}

void CommandTemplateManager::loadJSONDataManagerInternal(var data)
{
	BaseManager::loadJSONDataManagerInternal(data);
}
