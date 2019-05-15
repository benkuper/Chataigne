/*
  ==============================================================================

    GenericAppCommand.cpp
    Created: 15 May 2019 7:37:31pm
    Author:  bkupe

  ==============================================================================
*/

#include "GenericAppCommand.h"


GenericAppCommand::GenericAppCommand(ChataigneGenericModule* _module, CommandContext context, var params) :
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

void GenericAppCommand::triggerInternal()
{
	switch (type)
	{
	case NEW_SESSION:
	{

		MessageManagerLock mmLock;
		Engine::mainEngine->createNewGraph();
	}
	break;

	case OPEN_SESSION:
	{
		if(file->getFile().exists())
		{
			MessageManagerLock mmLock;
			Engine::mainEngine->loadDocument(file->getFile());
		}
		else
		{
			NLOGWARNING(niceName, "File doesn't exist : " << file->getFile().getFullPathName());
		}
	}

	case CLOSE_APP:
	{
		MessageManagerLock mmLock;
		OrganicApplication::quit();
	}
	break;
	}
}

BaseCommand* GenericAppCommand::create(ControllableContainer * module, CommandContext context, var params)
{
	return new GenericAppCommand((ChataigneGenericModule*)module, context, params);

}
