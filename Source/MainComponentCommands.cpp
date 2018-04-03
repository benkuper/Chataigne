/*
 ==============================================================================

 MainComponentMenuBar.cpp
 Created: 25 Mar 2016 6:02:02pm
 Author:  Martin Hermant

 ==============================================================================
 */


#include "MainComponent.h"
#include "Project/ProjectSettings.h"
#include "UI/AboutWindow.h"

namespace CommandIDs
{
	static const int open = 0x30000;
	static const int save = 0x30001;
	static const int saveAs = 0x30002;
	static const int newFile = 0x30003;
	static const int openLastDocument = 0x30004;
	static const int checkForUpdates = 0x30005;


	static const int editProjectSettings = 0x50001;
	static const int editGlobalSettings = 0x50002;

	static const int showAbout = 0x60000;
	static const int gotoWebsite = 0x60001;
	static const int gotoForum = 0x60002;

	//undo
	static const int undo = 0x40001;
	static const int redo = 0x40002;
	static const int duplicate = 0x40003;


	// range ids
	static const int lastFileStartID = 100; // 100 to 200 max



}




void MainContentComponent::getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) {
	const String category("General");


	switch (commandID)
	{
	case CommandIDs::newFile:
		result.setInfo("New", "Creates a new filter graph file", category, 0);
		result.defaultKeypresses.add(KeyPress('n', ModifierKeys::commandModifier, 0));
		break;

	case CommandIDs::open:
		result.setInfo("Open...", "Opens a filter graph file", category, 0);
		result.defaultKeypresses.add(KeyPress('o', ModifierKeys::commandModifier, 0));
		break;

	case CommandIDs::openLastDocument:
		result.setInfo("Open Last Document", "Opens a filter graph file", category, 0);
		result.defaultKeypresses.add(KeyPress('o', ModifierKeys::shiftModifier | ModifierKeys::commandModifier, 0));
		break;

	case CommandIDs::save:
		result.setInfo("Save", "Saves the current graph to a file", category, 0);
		result.defaultKeypresses.add(KeyPress('s', ModifierKeys::commandModifier, 0));
		break;

	case CommandIDs::saveAs:
		result.setInfo("Save As...",
			"Saves a copy of the current graph to a file",
			category, 0);
		result.defaultKeypresses.add(KeyPress('s', ModifierKeys::shiftModifier | ModifierKeys::commandModifier, 0));
		break;

	case CommandIDs::checkForUpdates:
		result.setInfo("Check for updates", "Check if updates are available and download latest software", category, 0);
		break;


	case CommandIDs::editProjectSettings:
		result.setInfo("Project Settings...", "Edit the settings related to this project", category, 0);
		result.defaultKeypresses.add(KeyPress(';', ModifierKeys::commandModifier, 0));
		break;

	case CommandIDs::editGlobalSettings:
		result.setInfo("Preferences", "Edit the general settings related to this application", category, 0);
		result.defaultKeypresses.add(KeyPress(',', ModifierKeys::commandModifier, 0));
		break;

	case CommandIDs::showAbout:
		result.setInfo("About...", "", category, 0);
		break;

	case CommandIDs::gotoWebsite:
		result.setInfo("Go to website", "", category, 0);
		break;
	case CommandIDs::gotoForum:
		result.setInfo("Go to forum", "", category, 0);
		break;

	case CommandIDs::undo:
		result.setInfo("Undo " + UndoMaster::getInstance()->getUndoDescription(), "Undo the last action", category, 0);
		result.defaultKeypresses.add(KeyPress('z', ModifierKeys::commandModifier, 0));
		result.setActive(UndoMaster::getInstance()->canUndo());
		break;

	case CommandIDs::redo:
		result.setInfo("Redo " + UndoMaster::getInstance()->getRedoDescription(), "Redo the undone action", category, 0);
		result.defaultKeypresses.add(KeyPress('z', ModifierKeys::shiftModifier | ModifierKeys::commandModifier, 0));
		result.defaultKeypresses.add(KeyPress('y', ModifierKeys::commandModifier, 0));
		result.setActive(UndoMaster::getInstance()->canRedo());
		break;

	case StandardApplicationCommandIDs::copy:
	{
		BaseItem * item = InspectableSelectionManager::mainSelectionManager->getInspectableAs<BaseItem>();
		result.setInfo(item != nullptr ? "Copy " + item->niceName : "Nothing to copy", "Copy the selected item", category, 0);
		result.defaultKeypresses.add(KeyPress('c', ModifierKeys::commandModifier, 0));
		result.setActive(item != nullptr);
	}
	break;

	case StandardApplicationCommandIDs::cut:
	{
		BaseItem * item = InspectableSelectionManager::mainSelectionManager->getInspectableAs<BaseItem>();
		result.setInfo(item != nullptr ? "Cut " + item->niceName : "Nothing to cut", "Cut the selected item", category, 0);
		result.defaultKeypresses.add(KeyPress('x', ModifierKeys::commandModifier, 0));
		result.setActive(item != nullptr);
	}
	break;

	case StandardApplicationCommandIDs::paste:
	{
		bool canPaste = SystemClipboard::getTextFromClipboard().isNotEmpty();
		result.setInfo(canPaste? "Paste":"Nothing to paste","Paste whatever is in the clipboard", category, 0);
		result.defaultKeypresses.add(KeyPress('v', ModifierKeys::commandModifier, 0));
		result.setActive(canPaste);
	}
	break;

	case CommandIDs::duplicate:
	{
		BaseItem * item = InspectableSelectionManager::mainSelectionManager->getInspectableAs<BaseItem>();
		result.setInfo(item != nullptr ? "Duplicate " + item->niceName : "Nothing to duplicate", "Duplicate the selected item", category, 0);
		result.defaultKeypresses.add(KeyPress('d', ModifierKeys::commandModifier, 0));
		result.setActive(item != nullptr);
	}
	break;


	default:
		JUCEApplication::getInstance()->getCommandInfo(commandID, result);
		break;
	}
}


void MainContentComponent::getAllCommands(Array<CommandID>& commands) {
	// this returns the set of all commands that this target can perform..
	const CommandID ids[] = {
	  CommandIDs::newFile,
	  CommandIDs::open,
	  CommandIDs::openLastDocument,
	  CommandIDs::save,
	  CommandIDs::saveAs,
	  CommandIDs::checkForUpdates,
	  StandardApplicationCommandIDs::quit,
	  StandardApplicationCommandIDs::copy,
	  StandardApplicationCommandIDs::cut,
	  StandardApplicationCommandIDs::paste,
	  CommandIDs::duplicate,
	  CommandIDs::editGlobalSettings,
	  CommandIDs::editProjectSettings,
	  CommandIDs::showAbout,
	  CommandIDs::gotoWebsite,
	  CommandIDs::gotoForum,
	  CommandIDs::undo,
	  CommandIDs::redo
	};

	commands.addArray(ids, numElementsInArray(ids));
}


PopupMenu MainContentComponent::getMenuForIndex(int /*topLevelMenuIndex*/, const String& menuName) {
	PopupMenu menu;

	if (menuName == "File")
	{
		// "File" menu
		menu.addCommandItem(&getCommandManager(), CommandIDs::newFile);
		menu.addCommandItem(&getCommandManager(), CommandIDs::open);
		menu.addCommandItem(&getCommandManager(), CommandIDs::openLastDocument);

		RecentlyOpenedFilesList recentFiles;
		recentFiles.restoreFromString(getAppProperties().getUserSettings()
			->getValue("recentFiles"));

		PopupMenu recentFilesMenu;
		recentFiles.createPopupMenuItems(recentFilesMenu, CommandIDs::lastFileStartID, true, true);
		menu.addSubMenu("Open recent file", recentFilesMenu);

		menu.addCommandItem(&getCommandManager(), CommandIDs::save);
		menu.addCommandItem(&getCommandManager(), CommandIDs::saveAs);
		menu.addSeparator();
		menu.addCommandItem(&getCommandManager(), CommandIDs::editProjectSettings);
		menu.addCommandItem(&getCommandManager(), CommandIDs::editGlobalSettings);
		menu.addSeparator();
		menu.addCommandItem(&getCommandManager(), CommandIDs::checkForUpdates);
		menu.addSeparator();
		menu.addCommandItem(&getCommandManager(), StandardApplicationCommandIDs::quit);


	} else if (menuName == "Edit")
	{
		menu.addCommandItem(&getCommandManager(), CommandIDs::undo);
		menu.addCommandItem(&getCommandManager(), CommandIDs::redo);
		menu.addSeparator();
		menu.addCommandItem(&getCommandManager(), StandardApplicationCommandIDs::copy);
		menu.addCommandItem(&getCommandManager(), StandardApplicationCommandIDs::cut);
		menu.addCommandItem(&getCommandManager(), StandardApplicationCommandIDs::paste);
		menu.addCommandItem(&getCommandManager(), CommandIDs::duplicate);

	} else if (menuName == "Panels")
	{
		return ShapeShifterManager::getInstance()->getPanelsMenu();

	} else if (menuName == "Help")
	{
		menu.addCommandItem(&getCommandManager(), CommandIDs::showAbout);
		menu.addSeparator();
		menu.addCommandItem(&getCommandManager(), CommandIDs::gotoWebsite);
		menu.addCommandItem(&getCommandManager(), CommandIDs::gotoForum);

	}

	return menu;
}

bool MainContentComponent::perform(const InvocationInfo& info) {

	switch (info.commandID)
	{


	case CommandIDs::newFile:
	{
		FileBasedDocument::SaveResult result = Engine::mainEngine->saveIfNeededAndUserAgrees();
		if (result == FileBasedDocument::SaveResult::userCancelledSave)
		{

		} else if (result == FileBasedDocument::SaveResult::failedToWriteToFile)
		{
			LOGERROR("Could not save the document (Failed to write to file)\nCancelled loading of the new document");
		} else
		{
			Engine::mainEngine->createNewGraph();
		}
	}
	break;

	case CommandIDs::open:
	{
		FileBasedDocument::SaveResult result = Engine::mainEngine->saveIfNeededAndUserAgrees();
		if (result == FileBasedDocument::SaveResult::userCancelledSave)
		{

		} else if (result == FileBasedDocument::SaveResult::failedToWriteToFile)
		{
			LOGERROR("Could not save the document (Failed to write to file)\nCancelled loading of the new document");
		} else
		{
			Engine::mainEngine->loadFromUserSpecifiedFile(true);
		}
	}
	break;

	case CommandIDs::openLastDocument:
	{
		FileBasedDocument::SaveResult result = Engine::mainEngine->saveIfNeededAndUserAgrees();
		if (result == FileBasedDocument::SaveResult::userCancelledSave)
		{
		} else if (result == FileBasedDocument::SaveResult::failedToWriteToFile)
		{
			LOGERROR("Could not save the document (Failed to write to file)\nCancelled loading of the new document");
		} else
		{
			Engine::mainEngine->loadFrom(Engine::mainEngine->getLastDocumentOpened(), true);
		}
	}
	break;

	case CommandIDs::save:
	{
		FileBasedDocument::SaveResult result = Engine::mainEngine->save(true, true);
		if (result == FileBasedDocument::savedOk) LOG("File saved !");
		else LOGERROR("Error saving file");
	}
	break;

	case CommandIDs::saveAs:
	{
		FileBasedDocument::SaveResult result = Engine::mainEngine->saveAs(File::nonexistent, true, true, true);
		if (result == FileBasedDocument::savedOk) LOG("File saved !");
		else LOGERROR("Error saving file");
	}
	break;

	case CommandIDs::checkForUpdates:
		AppUpdater::getInstance()->checkForUpdates();
		break;

	case CommandIDs::undo:
		UndoMaster::getInstance()->undo();
		break;

	case CommandIDs::redo:
		UndoMaster::getInstance()->redo();
		break;


	case StandardApplicationCommandIDs::copy:
	case StandardApplicationCommandIDs::cut:
	{
		BaseItem * item = InspectableSelectionManager::mainSelectionManager->getInspectableAs<BaseItem>();
		if (item != nullptr)
		{
			item->copy();
			if (info.commandID == StandardApplicationCommandIDs::cut) item->remove();
		}
	}

	break;

	case StandardApplicationCommandIDs::paste:
	{
		BaseItem * item = InspectableSelectionManager::mainSelectionManager->getInspectableAs<BaseItem>();
		if (item != nullptr) item->paste();
		else
		{
			BaseItem::Listener * m = InspectableSelectionManager::mainSelectionManager->getInspectableAs<BaseItem::Listener>();
			if (m != nullptr) m->askForPaste();
		}
	}
	break;

	case CommandIDs::duplicate:
	{
		BaseItem * item = InspectableSelectionManager::mainSelectionManager->getInspectableAs<BaseItem>();
		if (item != nullptr) item->duplicate();
	}
	break;

	case CommandIDs::editProjectSettings:
		ProjectSettings::getInstance()->selectThis();
		break;

	case CommandIDs::editGlobalSettings:
		GlobalSettings::getInstance()->selectThis();
		break;

	case CommandIDs::showAbout:
	{
		AboutWindow w;
		DialogWindow::showModalDialog("About", &w, getTopLevelComponent(), Colours::transparentBlack, true);
	}
	break;

	case CommandIDs::gotoWebsite:
		URL("http://benjamin.kuperberg.fr/chataigne").launchInDefaultBrowser();
		break;

	case CommandIDs::gotoForum:
		URL("http://benjamin.kuperberg.fr/chataigne/forum").launchInDefaultBrowser();
		break;

	default:
		DBG("no command found");
		return JUCEApplication::getInstance()->perform(info);
	}

	return true;
}

void MainContentComponent::menuItemSelected(int menuItemID, int topLevelMenuIndex)
{
	DBG("Menu item selected " << menuItemID << ", " << topLevelMenuIndex);

	String menuName = getMenuBarNames()[topLevelMenuIndex];

	if (menuName == "Panels")
	{
		ShapeShifterManager::getInstance()->handleMenuPanelCommand(menuItemID);

	} else if (isPositiveAndBelow(menuItemID - CommandIDs::lastFileStartID, 100)) {
		RecentlyOpenedFilesList recentFiles;
		recentFiles.restoreFromString(getAppProperties().getUserSettings()
			->getValue("recentFiles"));
		Engine::mainEngine->loadFrom(recentFiles.getFile(menuItemID - CommandIDs::lastFileStartID), true);
	}
}


StringArray MainContentComponent::getMenuBarNames() {
	const char* const names[] = { "File","Edit","Panels", "Help",nullptr };
	return StringArray(names);
}
