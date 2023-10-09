/*
  ==============================================================================

	BaseCommandHandler.cpp
	Created: 19 Jan 2017 6:42:31pm
	Author:  Ben

  ==============================================================================
*/

#include "BaseCommandHandler.h"
#include "CommandFactory.h"
#include "ui/BaseCommandHandlerEditor.h"

BaseCommandHandler::BaseCommandHandler(const String& name, CommandContext _context, Module* _lockedModule, Multiplex* multiplex) :
	BaseItem(name),
	MultiplexTarget(multiplex),
	context(_context),
	lockedModule(_lockedModule),
	trigger(nullptr),
	handlerNotifier(5)
{
	showWarningInUI = true;
	//if (!isMultiplexed())
	//{
	trigger = addTrigger("Trigger", "Trigger this command");
	trigger->hideInEditor = true;
	//}

	scriptObject.getDynamicObject()->setMethod("setCommand", &BaseCommandHandler::setCommandFromScript);
}

BaseCommandHandler::~BaseCommandHandler()
{
	clearItem();
}

void BaseCommandHandler::clearItem()
{
	BaseItem::clearItem();
	if (ModuleManager::getInstanceWithoutCreating() != nullptr) ModuleManager::getInstance()->removeBaseManagerListener(this);
	setCommand(nullptr);
}


void BaseCommandHandler::triggerCommand(int multiplexIndex)
{
	if (command != nullptr) command->trigger(multiplexIndex);
}

void BaseCommandHandler::setCommand(CommandDefinition* commandDef)
{
	if (!commandDefinition.wasObjectDeleted() && commandDefinition == commandDef) return;

	var prevCommandData;
	if (command != nullptr)
	{
		removeChildControllableContainer(command.get());
		prevCommandData = command->getJSONData();

		command->removeCommandListener(this);
		if (command->module != nullptr)
		{
			command->module->removeInspectableListener(this);

			if (command->module->templateManager != nullptr && !command->module->isClearing)
			{
				command->module->templateManager->removeBaseManagerListener(this);
			}

		}
		unregisterLinkedInspectable(command->module);
	}


	commandDefinition = commandDef;
	if (commandDef != nullptr && !Engine::mainEngine->isClearing && !isClearing) command.reset(commandDef->create(context, multiplex));
	else command.reset();

	if (command != nullptr)
	{
		addChildControllableContainer(command.get());

		if (!prevCommandData.isVoid()) command->loadPreviousCommandData(prevCommandData); //keep as much as similar parameter possible
		else if (!ghostCommandData.isVoid()) command->loadJSONData(ghostCommandData);
		//else if (!isCurrentlyLoadingData) setNiceName(commandDef->commandType);

		ghostModuleName = command->module->shortName;
		ghostCommandMenuPath = commandDef->menuPath;
		ghostCommandName = commandDef->commandType;
		ghostCommandData = var();

		command->addCommandListener(this);
		command->module->addInspectableListener(this);
		command->module->templateManager->removeBaseManagerListener(this);

		clearWarning();

		registerLinkedInspectable(command->module);


		if (ModuleManager::getInstanceWithoutCreating() != nullptr) ModuleManager::getInstance()->removeBaseManagerListener(this);
	}
	else if (ghostModuleName.isNotEmpty() && ghostCommandName.isNotEmpty())
	{
		if (!Engine::mainEngine->isClearing && !isClearing)
		{
			setWarningMessage("Command not found : " + ghostModuleName + ":" + ghostCommandName);
		}
	}


	if (!isClearing && !Engine::mainEngine->isClearing)
	{
		commandHandlerListeners.call(&CommandHandlerListener::commandChanged, this);
		handlerNotifier.addMessage(new CommandHandlerEvent(CommandHandlerEvent::COMMAND_CHANGED, this));
	}

	if (Engine::mainEngine != nullptr && !Engine::mainEngine->isLoadingFile && !Engine::mainEngine->isClearing) Engine::mainEngine->changed();
}


var BaseCommandHandler::getJSONData()
{
	var data = BaseItem::getJSONData();
	if (command != nullptr && !commandDefinition.wasObjectDeleted())
	{
		if (command->module != nullptr) data.getDynamicObject()->setProperty("commandModule", command->module->shortName);
		data.getDynamicObject()->setProperty("commandPath", commandDefinition->menuPath);
		data.getDynamicObject()->setProperty("commandType", commandDefinition->commandType);
		data.getDynamicObject()->setProperty("command", command->getJSONData());
	}
	else if (!ghostCommandData.isVoid())
	{
		data.getDynamicObject()->setProperty("ghostCommandData", ghostCommandData);
		data.getDynamicObject()->setProperty("ghostModuleName", ghostModuleName);
		data.getDynamicObject()->setProperty("ghostCommandMenuPath", ghostCommandMenuPath);
		data.getDynamicObject()->setProperty("ghostCommandName", ghostCommandName);
	}

	return data;
}

void BaseCommandHandler::loadJSONDataInternal(var data)
{
	BaseItem::loadJSONDataInternal(data);

	ghostCommandData = data.getProperty("ghostCommandData", var());
	ghostModuleName = data.getProperty("ghostModuleName", "");
	ghostCommandMenuPath = data.getProperty("ghostCommandMenuPath", "");
	ghostCommandName = data.getProperty("ghostCommandName", "");

	if (data.getDynamicObject()->hasProperty("commandModule"))
	{
		Module* m = ModuleManager::getInstance()->getModuleWithName(data.getProperty("commandModule", ""));
		if (m != nullptr)
		{
			String menuPath = data.getProperty("commandPath", "");
			String commandType = data.getProperty("commandType", "");
			setCommand(m->getCommandDefinitionFor(menuPath, commandType));
			if (command != nullptr)
			{
				command->loadJSONData(data.getProperty("command", var()));
			}
		}
		else
		{
			DBG("Output not found : " << data.getProperty("commandModule", "").toString());
		}
	}

	if (command == nullptr && ghostModuleName.isNotEmpty() && ModuleManager::getInstanceWithoutCreating() != nullptr)
	{
		ModuleManager::getInstance()->addBaseManagerListener(this);
	}

	if (command == nullptr && ghostCommandName.isNotEmpty())
	{
		setWarningMessage("Command not found : " + ghostModuleName + ":" + ghostCommandName);
	}
	else
	{
		clearWarning();
	}
}

void BaseCommandHandler::commandContentChanged()
{
	if (isClearing) return;
	commandHandlerListeners.call(&CommandHandlerListener::commandUpdated, this);
	handlerNotifier.addMessage(new CommandHandlerEvent(CommandHandlerEvent::COMMAND_UPDATED, this));
}

void BaseCommandHandler::commandTemplateDestroyed()
{
	if (command != nullptr && !Engine::mainEngine->isClearing)

	{
		ghostCommandData = command->getJSONData();
		//DBG("Template destroyed, command data = "+JSON::toString(ghostCommandData));
		if (command->module != nullptr && command->module->templateManager != nullptr) command->module->templateManager->addBaseManagerListener(this);
		if (!Engine::mainEngine->isClearing && ModuleManager::getInstanceWithoutCreating() != nullptr) ModuleManager::getInstance()->addBaseManagerListener(this);
	}
	setCommand(nullptr);
}

void BaseCommandHandler::onContainerTriggerTriggered(Trigger* t)
{
	BaseItem::onContainerTriggerTriggered(t);
	if (t == trigger) triggerCommand(getPreviewIndex());
}

void BaseCommandHandler::inspectableDestroyed(Inspectable*)
{
	if (command != nullptr) ghostCommandData = command->getJSONData();
	setCommand(nullptr);
	if (!isClearing && !Engine::mainEngine->isClearing && ModuleManager::getInstanceWithoutCreating() != nullptr) ModuleManager::getInstance()->addBaseManagerListener(this);
}

void BaseCommandHandler::itemAdded(Module* m)
{
	if (command == nullptr && m->shortName == ghostModuleName)
	{
		setCommand(m->getCommandDefinitionFor(ghostCommandMenuPath, ghostCommandName));
	}
}

void BaseCommandHandler::itemsAdded(Array<Module*> modules)
{
	for (auto& m : modules)
	{
		if (command == nullptr && m->shortName == ghostModuleName)
		{
			setCommand(m->getCommandDefinitionFor(ghostCommandMenuPath, ghostCommandName));
			return;
		}
	}
}

void BaseCommandHandler::itemAdded(CommandTemplate* t)
{
	if (command == nullptr && ghostCommandMenuPath == "Templates" && t->niceName == ghostCommandName)
	{
		Module* m = ModuleManager::getInstance()->getItemWithName(ghostModuleName);
		if (m != nullptr) setCommand(m->getCommandDefinitionFor(ghostCommandMenuPath, ghostCommandName));
	}
}

void BaseCommandHandler::itemsAdded(Array<CommandTemplate*> templates)
{
	for (auto& t : templates)
	{
		if (command == nullptr && ghostCommandMenuPath == "Templates" && t->niceName == ghostCommandName)
		{
			Module* m = ModuleManager::getInstance()->getItemWithName(ghostModuleName);
			if (m != nullptr)
			{
				setCommand(m->getCommandDefinitionFor(ghostCommandMenuPath, ghostCommandName));
				return;
			}
		}
	}
}

var BaseCommandHandler::setCommandFromScript(const var::NativeFunctionArgs& a)
{
	BaseCommandHandler* h = getObjectFromJS<BaseCommandHandler>(a);

	if (h == nullptr) return var();
	if (!checkNumArgs("Command", a, 3)) return var();

	Module* m = ModuleManager::getInstance()->getModuleWithName(a.arguments[0].toString());
	if (m != nullptr)
	{
		String menuPath = a.arguments[1].toString();
		String commandType = a.arguments[2].toString();
		h->setCommand(m->getCommandDefinitionFor(menuPath, commandType));
		return h->command->getScriptObject();
	}

	return var();
}

InspectableEditor* BaseCommandHandler::getEditorInternal(bool isRoot, Array<Inspectable*> inspectables)
{
	return new BaseCommandHandlerEditor(this, isRoot);
}
