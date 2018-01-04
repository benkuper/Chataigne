/*
  ==============================================================================

    BaseCommandHandlerManagerUI.h
    Created: 2 Jan 2018 6:19:33pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "BaseCommandHandler.h"
#include "CommandFactory.h"

template <class T>
class BaseCommandHandlerManagerEditor :
	public GenericManagerEditor<T>
{
public:
	BaseCommandHandlerManagerEditor(BaseManager<T> * manager, CommandContext context,bool isRoot) :
		GenericManagerEditor(manager,isRoot),
		context(context)
	{

	}

	CommandContext context;

	~BaseCommandHandlerManagerEditor() {}

	
	void showMenuAndAddItem(bool /*isFromAddButton*/) override
	{
		CommandDefinition * def = CommandFactory::showMenuAndGetCommand(context);
		if (def == nullptr) return;
		else
		{
			T * item = manager->BaseManager<T>::addItem();
			BaseCommandHandler * c = dynamic_cast<BaseCommandHandler *>(item);
			c->setCommand(def);
		}
	}
};