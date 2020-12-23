/*
  ==============================================================================

    GenericAppCommand.h
    Created: 15 May 2019 7:37:31pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once
#include "Common/Command/BaseCommand.h"
#include "../ChataigneGenericModule.h"


class GenericAppCommand :
	public BaseCommand
{
public:
	GenericAppCommand(ChataigneGenericModule* _module, CommandContext context, var params, IteratorProcessor * iterator = nullptr);
	~GenericAppCommand();

	enum Type { NEW_SESSION, OPEN_SESSION, CLOSE_APP };
	Type type;

	FileParameter* file;

	void triggerInternal() override;

	static BaseCommand* create(ControllableContainer* module, CommandContext context, var params, IteratorProcessor * iterator = nullptr);
};
