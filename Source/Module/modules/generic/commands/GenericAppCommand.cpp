/*
  ==============================================================================

	GenericAppCommand.cpp
	Created: 15 May 2019 7:37:31pm
	Author:  bkupe

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

GenericAppCommand::GenericAppCommand(ChataigneGenericModule* _module, CommandContext context, var params, Multiplex* multiplex) :
	BaseCommand(_module, context, params, multiplex)
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

void GenericAppCommand::triggerInternal(int multiplexIndex)
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
		if (file->getFile().exists())
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

	case SAVE_SESSION:
		if (Engine::mainEngine->getFile().exists())
		{
			juce::Result r = Engine::mainEngine->saveDocument(Engine::mainEngine->getFile());
			if (r.ok()) LOG("Session saved");
			else if (r.failed()) LOGERROR("Session save error : " << r.getErrorMessage());
		}
		else
		{
			LOGWARNING("Please save first manually to use the save session command");
		}
		break;

	case CLOSE_APP:
	{
		MessageManager::callAsync([]() { OrganicApplication::quit(); });
	}

	case MINIMIZE:
	{
		MessageManager::callAsync([]() { getApp().mainWindow->setMinimised(true); });
		break;
	}

	case MAXIMIZE:
	{
		MessageManager::callAsync([]() { getApp().mainWindow->setMinimised(false); });
		break;
	}

	case CLOSE_TO_TRAY:
	{
		MessageManager::callAsync([]() { getApp().mainWindow->closeToTray(); });
		break;
	}
	break;

	case OPEN_FROM_TRAY:
	{
		MessageManager::callAsync([]() { getApp().mainWindow->openFromTray(); });
		break;
	}
	break;
	}
}

BaseCommand* GenericAppCommand::create(ControllableContainer* module, CommandContext context, var params, Multiplex* multiplex)
{
	return new GenericAppCommand((ChataigneGenericModule*)module, context, params, multiplex);

}
