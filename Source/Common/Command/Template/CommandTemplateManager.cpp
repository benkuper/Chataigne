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
	defManager.add(CommandDefinition::createDef(module, menuName, item->niceName, &BaseCommand::create)->addParam("template",item->shortName)); 
	item->addCommandTemplateListener(this);
}

void CommandTemplateManager::removeItemInternal(CommandTemplate * item)
{
	CommandDefinition * d = defManager.getCommandDefinitionFor(menuName, item->niceName); 
	defManager.remove(d);
	item->removeCommandTemplateListener(this);
}

CommandTemplate * CommandTemplateManager::addItemFromData(var data, bool addToUndo)
{
	CommandTemplate * ct = new CommandTemplate(module, data);
	return addItem(ct, data, addToUndo);
}

void CommandTemplateManager::setupTemplateDefinition()
{
	factory.defs.clear();
	for (auto &d : module->defManager.definitions)
	{
		factory.defs.add(Factory<CommandTemplate>::Definition::createDef(d->menuPath, d->commandType, CommandTemplate::create)
			->addParam("module", module->shortName)
			->addParam("menuPath", d->menuPath)->addParam("commandType", d->commandType));
	}
}

void CommandTemplateManager::templateNameChanged(CommandTemplate * ct)
{
	if (Engine::mainEngine->isLoadingFile || Engine::mainEngine->isClearing) return;
	CommandDefinition * def = defManager.definitions[items.indexOf(ct)];
	jassert(def != nullptr);
	
	def->commandType = ct->niceName;
	def->params.getDynamicObject()->setProperty("template", ct->shortName);
}
