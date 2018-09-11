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

BaseCommandHandler::BaseCommandHandler(const String & name, CommandContext _context, Module * _lockedModule) :
	BaseItem(name),
	context(_context),
	lockedModule(_lockedModule)
{
	trigger = addTrigger("Trigger", "Trigger this consequence");
	trigger->hideInEditor = true;
}

BaseCommandHandler::~BaseCommandHandler()
{
	setCommand(nullptr);
}

void BaseCommandHandler::triggerCommand()
{
	if (command != nullptr) command->trigger();
}

void BaseCommandHandler::setCommand(CommandDefinition * commandDef)
{
	var oldData = var();
	if (command != nullptr)
	{
		removeChildControllableContainer(command);
		command->module->removeInspectableListener(this);
		command->removeCommandListener(this);
		oldData = command->getJSONData();
	}

	commandDefinition = commandDef;
	if (commandDef != nullptr) command = commandDef->create(context);
	else command = nullptr;

	if (command != nullptr)
	{
		command->hideEditorHeader = true;
		addChildControllableContainer(command);
		command->module->addInspectableListener(this);
		command->addCommandListener(this);
		command->loadPreviousCommandData(oldData); //keep as much as similar parameter possible
	}

	commandHandlerListeners.call(&CommandHandlerListener::commandChanged, this);
}


var BaseCommandHandler::getJSONData()
{
	var data = BaseItem::getJSONData();
	if (command != nullptr && !commandDefinition.wasObjectDeleted())
	{
		if(command->module != nullptr) data.getDynamicObject()->setProperty("commandModule", command->module->shortName);
		data.getDynamicObject()->setProperty("commandPath", commandDefinition->menuPath);
		data.getDynamicObject()->setProperty("commandType", commandDefinition->commandType);
		data.getDynamicObject()->setProperty("command", command->getJSONData());
	}

	return data;
}

void BaseCommandHandler::loadJSONDataInternal(var data)
{
	BaseItem::loadJSONDataInternal(data);
	if (data.getDynamicObject()->hasProperty("commandModule"))
	{
		Module * m = ModuleManager::getInstance()->getModuleWithName(data.getProperty("commandModule",""));
		if (m != nullptr)
		{
			String menuPath = data.getProperty("commandPath", "");
			String commandType = data.getProperty("commandType", "");
			setCommand(m->getCommandDefinitionFor(menuPath, commandType));
			if (command != nullptr)
			{
				command->loadJSONData(data.getProperty("command", var()));
			}
		} else
		{
			DBG("Output not found : " << data.getProperty("commandModule", "").toString());
		}
	}
}



void BaseCommandHandler::onContainerTriggerTriggered(Trigger * t)
{
	if (t == trigger)
	{
		triggerCommand();
	}
}

void BaseCommandHandler::commandContentChanged()
{
	commandHandlerListeners.call(&CommandHandlerListener::commandUpdated, this);
}

void BaseCommandHandler::inspectableDestroyed(Inspectable *)
{
	setCommand(nullptr);
}

InspectableEditor * BaseCommandHandler::getEditor(bool isRoot)
{
	return new BaseCommandHandlerEditor(this,isRoot);
}
