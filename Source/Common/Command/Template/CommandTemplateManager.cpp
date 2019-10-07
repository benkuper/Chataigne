/*
  ==============================================================================

    CommandTemplateManager.cpp
    Created: 31 May 2018 11:29:27am
    Author:  Ben

  ==============================================================================
*/

#include "CommandTemplateManager.h"
#include "Module/Module.h"

CommandTemplateManager::CommandTemplateManager(Module * module) :
	BaseManager("Templates"),
	module(module)
{
	managerFactory = &factory;
	selectItemWhenCreated = false;
}

CommandTemplateManager::~CommandTemplateManager()
{
}

void CommandTemplateManager::addItemInternal(CommandTemplate * item, var)
{
	setupTemplatesDefinitions();
	item->addCommandTemplateListener(this);
}

void CommandTemplateManager::removeItemInternal(CommandTemplate * item)
{
	setupTemplatesDefinitions();
	item->removeCommandTemplateListener(this);
}

CommandTemplate * CommandTemplateManager::addItemFromData(var data, bool addToUndo)
{
	CommandTemplate * ct = new CommandTemplate(module, data);
	return addItem(ct, data, addToUndo);
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

void CommandTemplateManager::setupTemplatesDefinitions()
{
	defManager.clear();
	for (auto& item : items)
	{
		defManager.add(CommandDefinition::createDef(module, menuName, item->niceName, &BaseCommand::create)->addParam("template", item->shortName));
	}
}

void CommandTemplateManager::reorderItems()
{
	BaseManager::reorderItems();
	if(!isCurrentlyLoadingData) setupTemplatesDefinitions();
}

void CommandTemplateManager::templateNameChanged(CommandTemplate * ct)
{
	if (!isCurrentlyLoadingData && Engine::mainEngine->isLoadingFile || Engine::mainEngine->isClearing) return;
	if(!isCurrentlyLoadingData) setupTemplatesDefinitions();
}

void CommandTemplateManager::loadJSONDataManagerInternal(var data)
{
	BaseManager::loadJSONDataManagerInternal(data);
	setupTemplatesDefinitions();
}
