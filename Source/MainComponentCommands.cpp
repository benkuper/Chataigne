/*
 ==============================================================================

 MainComponentMenuBar.cpp
 Created: 25 Mar 2016 6:02:02pm
 Author:  Martin Hermant

 ==============================================================================
 */


#include "MainComponent.h"
#include "Engine.h"

namespace CommandIDs
{
  static const int open                   = 0x30000;
  static const int save                   = 0x30001;
  static const int saveAs                 = 0x30002;
  static const int newFile                = 0x30003;
  static const int openLastDocument       = 0x30004;

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
    CommandIDs::saveAs
  };

  commands.addArray (ids, numElementsInArray (ids));
}


PopupMenu MainContentComponent::getMenuForIndex (int /*topLevelMenuIndex*/, const String& menuName) {
  PopupMenu menu;

  if(menuName == "File")
  {
    // "File" menu
    menu.addCommandItem (&getCommandManager(), CommandIDs::newFile);
    menu.addCommandItem (&getCommandManager(), CommandIDs::open);
    menu.addCommandItem(&getCommandManager(), CommandIDs::openLastDocument);

    RecentlyOpenedFilesList recentFiles;
    recentFiles.restoreFromString (getAppProperties().getUserSettings()
                                   ->getValue ("recentFiles"));

    PopupMenu recentFilesMenu;
    recentFiles.createPopupMenuItems (recentFilesMenu, CommandIDs::lastFileStartID, true, true);
    menu.addSubMenu ("Open recent file", recentFilesMenu);

    menu.addCommandItem (&getCommandManager(), CommandIDs::save);
    menu.addCommandItem (&getCommandManager(), CommandIDs::saveAs);
    menu.addSeparator();
    menu.addCommandItem (&getCommandManager(), StandardApplicationCommandIDs::quit);

  } else if (menuName == "Panels")
  {
    return ShapeShifterManager::getInstance()->getPanelsMenu();
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
        if (result == 1) engine->save(true, true);
        engine->createNewGraph();

      }
    }
      break;

    case CommandIDs::open:
    {
      int result = AlertWindow::showYesNoCancelBox(AlertWindow::QuestionIcon, "Save document", "Do you want to save the document before opening a new one ?");
      if (result != 0)
      {
		if (result == 1) engine->save(true, true);
		engine->loadFromUserSpecifiedFile(true);
      }
    }
      break;

    case CommandIDs::openLastDocument:
    {
      // TODO implement the JUCE version calling change every time something is made (maybe todo with undomanager)
      //			int result = engine->saveIfNeededAndUserAgrees();
      int result = AlertWindow::showYesNoCancelBox(AlertWindow::QuestionIcon, "Save document", "Do you want to save the document before opening the last one ?");
      if (result != 0)
      {
        if (result == 1) engine->save(true, true);
        engine->loadFrom(engine->getLastDocumentOpened(),true);
      }
    }
      break;

    case CommandIDs::save:
      engine->save (true, true);
      break;

    case CommandIDs::saveAs:
      engine->saveAs (File::nonexistent, true, true, true);
      break;


    default:
      DBG("no command found");
      return false;
  }

  return true;
}

void MainContentComponent::menuItemSelected(int menuItemID, int topLevelMenuIndex)
{

  String menuName = getMenuBarNames()[topLevelMenuIndex];
  if (menuName == "Panels")
  {
    ShapeShifterManager::getInstance()->handleMenuPanelCommand(menuItemID);
  }
  else if (isPositiveAndBelow(menuItemID-CommandIDs::lastFileStartID, 100)){
    RecentlyOpenedFilesList recentFiles;
    recentFiles.restoreFromString (getAppProperties().getUserSettings()
                                   ->getValue ("recentFiles"));
    engine->loadFrom(recentFiles.getFile(menuItemID-CommandIDs::lastFileStartID),true);
  }
}


StringArray MainContentComponent::getMenuBarNames() {
  const char* const names[] = { "File","Panels", nullptr };
  return StringArray (names);
}
