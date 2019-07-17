/*
 ==============================================================================

 MainComponentMenuBar.cpp
 Created: 25 Mar 2016 6:02:02pm
 Author:  Martin Hermant

 ==============================================================================
 */


#include "MainComponent.h"
#include "UI/AboutWindow.h"
#include "Guider/Guider.h"
#include "Module/Community/CommunityModuleManager.h"
#include "Module/ModuleFactory.h"

namespace ChataigneCommandIDs
{
	static const int showAbout = 0x60000;
	static const int gotoWebsite = 0x60001;
	static const int gotoForum = 0x60002;
	static const int gotoDocs = 0x60003;
	static const int postGithubIssue = 0x60004;
	static const int donate = 0x60005;
	static const int playPauseSequenceEditor = 0x80000;
	static const int guideStart = 0x300; //up to 0x300 +100
	static const int exitGuide = 0x399; 
	static const int goToCommunityModules = 0x500;
	static const int reloadCustomModules = 0x501;
	
}

void MainContentComponent::getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) 
{
	if (commandID >= ChataigneCommandIDs::guideStart && commandID < ChataigneCommandIDs::guideStart + 99)
	{
		result.setInfo(Guider::getInstance()->getGuideName(commandID - ChataigneCommandIDs::guideStart), "", "Guides", result.readOnlyInKeyEditor);
		return;
	}

	switch (commandID)
	{
	case ChataigneCommandIDs::showAbout:
		result.setInfo("About...", "", "General", result.readOnlyInKeyEditor);
		break;

	case ChataigneCommandIDs::donate:
		result.setInfo("Be cool and donate", "", "General", result.readOnlyInKeyEditor);
		break;

	case ChataigneCommandIDs::gotoWebsite:
		result.setInfo("Go to website", "", "Help", result.readOnlyInKeyEditor);
		break;
	case ChataigneCommandIDs::gotoForum:
		result.setInfo("Go to forum", "", "Help", result.readOnlyInKeyEditor);
		break;

	case ChataigneCommandIDs::gotoDocs:
		result.setInfo("Go to the Amazing Documentation", "", "Help", result.readOnlyInKeyEditor);
		break;

	case ChataigneCommandIDs::postGithubIssue:
		result.setInfo("Post an issue on github", "", "Help", result.readOnlyInKeyEditor);
		break;

	case ChataigneCommandIDs::playPauseSequenceEditor:
		result.setInfo("Play/Pause Sequence", "", "Control", 0);
		result.addDefaultKeypress(KeyPress::spaceKey, ModifierKeys::noModifiers);
		break;

	case ChataigneCommandIDs::goToCommunityModules:
		result.setInfo("Community Modules Manager", "", "General", result.readOnlyInKeyEditor);
		break;

	case ChataigneCommandIDs::reloadCustomModules:
		result.setInfo("Reload Custom Modules", "", "General", result.readOnlyInKeyEditor);
		break;

	case ChataigneCommandIDs::exitGuide:
		result.setInfo("Exit current guide", "", "Guides", result.readOnlyInKeyEditor);
		result.addDefaultKeypress(KeyPress::escapeKey, ModifierKeys::noModifiers);
		result.setActive(Guider::getInstance()->guide != nullptr);
		break;

	default:
		OrganicMainContentComponent::getCommandInfo(commandID, result);
		break;
	}
}



void MainContentComponent::getAllCommands(Array<CommandID>& commands) {

	OrganicMainContentComponent::getAllCommands(commands);

	const CommandID ids[] = {

		ChataigneCommandIDs::showAbout,
		ChataigneCommandIDs::donate,
		ChataigneCommandIDs::gotoWebsite,
		ChataigneCommandIDs::gotoForum,
		ChataigneCommandIDs::gotoDocs,
		ChataigneCommandIDs::postGithubIssue,
		ChataigneCommandIDs::playPauseSequenceEditor,
		ChataigneCommandIDs::goToCommunityModules,
		ChataigneCommandIDs::reloadCustomModules,
		ChataigneCommandIDs::exitGuide,
	};

	commands.addArray(ids, numElementsInArray(ids));
	for (int i = 0; i < Guider::getInstance()->factory.defs.size(); i++) commands.add(ChataigneCommandIDs::guideStart + i);
}


PopupMenu MainContentComponent::getMenuForIndex(int topLevelMenuIndex, const String& menuName) 
{
	PopupMenu menu = OrganicMainContentComponent::getMenuForIndex(topLevelMenuIndex, menuName);

	if (menuName == "Control")
	{
		menu.addCommandItem(&getCommandManager(), ChataigneCommandIDs::playPauseSequenceEditor);
	}else if (menuName == "Help")
	{
		menu.addCommandItem(&getCommandManager(), ChataigneCommandIDs::showAbout);
		menu.addCommandItem(&getCommandManager(), ChataigneCommandIDs::donate);
		menu.addSeparator();
		menu.addCommandItem(&getCommandManager(), ChataigneCommandIDs::gotoWebsite);
		menu.addCommandItem(&getCommandManager(), ChataigneCommandIDs::gotoForum);
		menu.addCommandItem(&getCommandManager(), ChataigneCommandIDs::gotoDocs);
		menu.addCommandItem(&getCommandManager(), ChataigneCommandIDs::postGithubIssue);

	}else if (menuName == "Guides")
	{
		for (int i = 0; i < Guider::getInstance()->factory.defs.size(); i++)
		{
			menu.addCommandItem(&getCommandManager(), ChataigneCommandIDs::guideStart + i);
		}

		menu.addSeparator();
		menu.addCommandItem(&getCommandManager(), ChataigneCommandIDs::exitGuide);
	}

	return menu;
}

void MainContentComponent::fillFileMenuInternal(PopupMenu & menu)
{
	menu.addCommandItem(&getCommandManager(), ChataigneCommandIDs::goToCommunityModules);
	menu.addCommandItem(&getCommandManager(), ChataigneCommandIDs::reloadCustomModules);
}

bool MainContentComponent::perform(const InvocationInfo& info)
{

	if (info.commandID >= ChataigneCommandIDs::guideStart && info.commandID < ChataigneCommandIDs::guideStart + 100)
	{
		Guider::getInstance()->launchGuideAtIndex(info.commandID - ChataigneCommandIDs::guideStart);
		return true;
	}

	switch (info.commandID)
	{


	case ChataigneCommandIDs::showAbout:
	{
		AboutWindow w;
		DialogWindow::showModalDialog("About", &w, getTopLevelComponent(), Colours::transparentBlack, true);
	}
	break;

	case ChataigneCommandIDs::donate:
		URL("https://www.paypal.me/benkuper").launchInDefaultBrowser();
		break;

	case ChataigneCommandIDs::gotoWebsite:
		URL("http://benjamin.kuperberg.fr/chataigne").launchInDefaultBrowser();
		break;

	case ChataigneCommandIDs::gotoForum:
		URL("http://benjamin.kuperberg.fr/chataigne/forum").launchInDefaultBrowser();
		break;

	case ChataigneCommandIDs::gotoDocs:
		URL("http://benjamin.kuperberg.fr/chataigne/docs").launchInDefaultBrowser();
		break;

	case ChataigneCommandIDs::postGithubIssue:
		URL("http://github.com/benkuper/Chataigne/issues").launchInDefaultBrowser();
		break;


	case ChataigneCommandIDs::goToCommunityModules:
		CommunityModuleManager::getInstance()->selectThis();
		break;

	case ChataigneCommandIDs::reloadCustomModules:
		ModuleFactory::getInstance()->updateCustomModules();
		break;

	case ChataigneCommandIDs::playPauseSequenceEditor:	
	{
		ShapeShifterContent * sContent = ShapeShifterManager::getInstance()->getContentForName("Sequence Editor");
		if (sContent != nullptr)
		{
			Component * c = sContent->contentComponent;
			TimeMachineView * tmw = dynamic_cast<TimeMachineView *>(c);
			if (tmw != nullptr)
			{
				SequenceEditorView * se = tmw->editor.get();
				if (se != nullptr)
				{
					if (se->sequence != nullptr) se->sequence->togglePlayTrigger->trigger();
				}
			}
		}
	}
	break;

	case ChataigneCommandIDs::exitGuide:
		Guider::getInstance()->setCurrentGuide(nullptr);
		break;

	default:
		return OrganicMainContentComponent::perform(info);
	}

	return true;
}

StringArray MainContentComponent::getMenuBarNames()
{
	StringArray names = OrganicMainContentComponent::getMenuBarNames();
	names.add("Control");
	names.add("Guides");
	names.add("Help");
	return names;
}
