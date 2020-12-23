/*
  ==============================================================================

    GenericAppCommand.cpp
    Created: 15 May 2019 7:37:31pm
    Author:  bkupe

  ==============================================================================
*/

#include "GenericAppCommand.h"


GenericAppCommand::GenericAppCommand(ChataigneGenericModule* _module, CommandContext context, var params, IteratorProcessor * iterator) :
	BaseCommand(_module, context, params)
{
	type = (Type)(int)(params.getProperty("type", NEW_SESSION));

	if (type == OPEN_SESSION)
	{
		file = addFileParameter("File", "The file to open. This will replace this session !");
		file->fileTypeFilter = "*.noisette";
	}
}

GenericAppCommand::~GenericAppCommand()
{
}

void GenericAppCommand::triggerInternal(int iterationIndex)
{
	switch (type)
	{
	case NEW_SESSION:
	{
		auto newGraphFunc = std::bind(&Engine::createNewGraph, Engine::mainEngine);
		Timer::callAfterDelay(.01f, newGraphFunc); //force timer to avoid clearing everything inside the trigger func which will bubble up after everything has been deleted
	}
	break;

	case OPEN_SESSION:
	{
		if(file->getFile().exists())
		{
			auto loadFileFunc = std::bind(&Engine::loadDocument, Engine::mainEngine, file->getFile());
			Timer::callAfterDelay(.01f, loadFileFunc); //force timer to avoid clearing everything inside the trigger func which will bubble up after everything has been deleted
		}
		else
		{
			NLOGWARNING(niceName, "File doesn't exist : " << file->getFile().getFullPathName());
		}
	}
	break;

	case CLOSE_APP:
	{
		MessageManagerLock mmLock;
		OrganicApplication::quit();
	}
	break;
	}
}

BaseCommand* GenericAppCommand::create(ControllableContainer * module, CommandContext context, var params, IteratorProcessor * iterator)
{
	return new GenericAppCommand((ChataigneGenericModule*)module, context, params, iterator);

}
