/*
 ==============================================================================

 MainComponentMenuBar.cpp
 Created: 25 Mar 2016 6:02:02pm
 Author:  Martin Hermant

 ==============================================================================
 */


#include "MainComponent.h"

namespace CommandIDs
{
  static const int open                   = 0x30000;
  static const int save                   = 0x30001;
  static const int saveAs                 = 0x30002;
  static const int newFile                = 0x30003;
  static const int openLastDocument       = 0x30004;
  static const int checkForUpdates		  = 0x30005;

  //undo
  static const int undo = 0x40001;
  static const int redo = 0x40002;

  // range ids
  static const int lastFileStartID        =100; // 100 to 200 max



}




void MainContentComponent::getCommandInfo (CommandID commandID, ApplicationCommandInfo& result)  {
  const String category ("General");

  switch (commandID)
  {
    case CommandIDs::newFile:
      result.setInfo ("New", "Creates a new filter graph file", category, 0);
      result.defaultKeypresses.add(KeyPress('n', ModifierKeys::commandModifier, 0));
      break;

    case CommandIDs::open:
      result.setInfo ("Open...", "Opens a filter graph file", category, 0);
      result.defaultKeypresses.add (KeyPress ('o', ModifierKeys::commandModifier, 0));
      break;

    case CommandIDs::openLastDocument:
      result.setInfo("Open Last Document", "Opens a filter graph file", category, 0);
      result.defaultKeypresses.add(KeyPress('o', ModifierKeys::shiftModifier | ModifierKeys::commandModifier, 0));
      break;

    case CommandIDs::save:
      result.setInfo ("Save", "Saves the current graph to a file", category, 0);
      result.defaultKeypresses.add (KeyPress ('s', ModifierKeys::commandModifier, 0));
      break;

    case CommandIDs::saveAs:
      result.setInfo ("Save As...",
                      "Saves a copy of the current graph to a file",
                      category, 0);
      result.defaultKeypresses.add (KeyPress ('s', ModifierKeys::shiftModifier | ModifierKeys::commandModifier, 0));
	  break;

	case CommandIDs::checkForUpdates:
		result.setInfo("Check for updates", "Check if updates are available and download latest software", category, 0);
		break;

	case CommandIDs::undo:
		//result.setInfo("Undo " + UndoMaster::getInstance()->getUndoDescription(),"Undo the last action", category, 0);
		//result.defaultKeypresses.add(KeyPress('z', ModifierKeys::ctrlModifier, 0));
		break;
		 
	case CommandIDs::redo:
		//result.setInfo("Redo " + UndoMaster::getInstance()->getRedoDescription(), "Redo the undone action", category, 0);
		//result.defaultKeypresses.add(KeyPress('z', ModifierKeys::shiftModifier | ModifierKeys::commandModifier, 0));
		//result.defaultKeypresses.add(KeyPress('y', ModifierKeys::ctrlModifier, 0));
		break;

    default:
      break;
  }
}


void MainContentComponent::getAllCommands (Array<CommandID>& commands) {
  // this returns the set of all commands that this target can perform..
  const CommandID ids[] = {
    CommandIDs::newFile,
    CommandIDs::open,
    CommandIDs::openLastDocument,
    CommandIDs::save,
    CommandIDs::saveAs,
	CommandIDs::checkForUpdates
	//CommandIDs::undo,
	//CommandIDs::redo
  };

  commands.addArray (ids, numElementsInArray (ids));
}


PopupMenu MainContentComponent::getMenuForIndex (int /*topLevelMenuIndex*/, const String& menuName) {
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
	  menu.addCommandItem(&getCommandManager(), StandardApplicationCommandIDs::quit);
  }else if(menuName =="Edit")
  {
	  menu.addCommandItem(&getCommandManager(), CommandIDs::undo);
	  menu.addCommandItem(&getCommandManager(), CommandIDs::redo);

  } else if (menuName == "Panels")
  {
    return ShapeShifterManager::getInstance()->getPanelsMenu();
  }
  else if (menuName == "Options")
  {
	  menu.addCommandItem(&getCommandManager(), CommandIDs::checkForUpdates);
  }

  return menu;
}

bool MainContentComponent::perform(const InvocationInfo& info) {

  switch (info.commandID)
  {


    case CommandIDs::newFile:
    {
      int result = AlertWindow::showYesNoCancelBox(AlertWindow::QuestionIcon, "Save document", "Do you want to save the document before creating a new one ?");
      if (result != 0)
      {
        if (result == 1) Engine::mainEngine->save(true, true);
        Engine::mainEngine->createNewGraph();

      }
    }
      break;

    case CommandIDs::open:
    {
      int result = AlertWindow::showYesNoCancelBox(AlertWindow::QuestionIcon, "Save document", "Do you want to save the document before opening a new one ?");
      if (result != 0)
      {
		if (result == 1) Engine::mainEngine->save(true, true);
		Engine::mainEngine->loadFromUserSpecifiedFile(true);
      }
    }
      break;

    case CommandIDs::openLastDocument:
    {
      // TODO implement the JUCE version calling change every time something is made (maybe todo with undomanager)
      //			int result = Engine::mainEngine->saveIfNeededAndUserAgrees();
      int result = AlertWindow::showYesNoCancelBox(AlertWindow::QuestionIcon, "Save document", "Do you want to save the document before opening the last one ?");
      if (result != 0)
      {
        if (result == 1) Engine::mainEngine->save(true, true);
        Engine::mainEngine->loadFrom(Engine::mainEngine->getLastDocumentOpened(),true);
      }
    }
      break;

    case CommandIDs::save:
      Engine::mainEngine->save (true, true);
      break;

    case CommandIDs::saveAs:
      Engine::mainEngine->saveAs (File::nonexistent, true, true, true);
      break;

	case CommandIDs::checkForUpdates:
		AppUpdater::getInstance()->checkForUpdates();
		break;

	case CommandIDs::undo:
		//UndoMaster::getInstance()->undo();
		break;

	case CommandIDs::redo:
		//UndoMaster::getInstance()->redo();
		break;

    default:
      DBG("no command found");
      return false;
  }

  return true;
}

void MainContentComponent::menuItemSelected(int menuItemID, int topLevelMenuIndex)
{
#if JUCE_MAC
    topLevelMenuIndex--; //On mac, first menu index is 1 (because of the App menu);
#endif
    
      String menuName = getMenuBarNames()[topLevelMenuIndex];

  if (menuName == "Panels")
  {
    ShapeShifterManager::getInstance()->handleMenuPanelCommand(menuItemID);
  }
  else if (isPositiveAndBelow(menuItemID-CommandIDs::lastFileStartID, 100)){
    RecentlyOpenedFilesList recentFiles;
    recentFiles.restoreFromString (getAppProperties().getUserSettings()
                                   ->getValue ("recentFiles"));
    Engine::mainEngine->loadFrom(recentFiles.getFile(menuItemID-CommandIDs::lastFileStartID),true);
  }
}


StringArray MainContentComponent::getMenuBarNames() {
  const char* const names[] = { "File","Edit", "Panels", "Options",nullptr };
  return StringArray (names);
}
