#include "MainComponent.h"
/*
 ==============================================================================

 MainComponentMenuBar.cpp
 Created: 25 Mar 2016 6:02:02pm
 Author:  Martin Hermant

 ==============================================================================
 */


namespace ChataigneCommandIDs
{
	static const int showAbout = 0x60000;
	static const int gotoWebsite = 0x60001;
	static const int gotoDiscord = 0x60002;
	static const int gotoDocs = 0x60003;
	static const int postGithubIssue = 0x60004;
	static const int donate = 0x60005;
	static const int sponsor = 0x60055;
	static const int showWelcome = 0x60006;
	static const int gotoChangelog = 0x60007;

	static const int guideStart = 0x300; //up to 0x300 +100
	static const int exitGuide = 0x399;
	static const int goToCommunityModules = 0x500;
	static const int reloadCustomModules = 0x501;
	static const int exportSelection = 0x800;
	static const int importSelection = 0x801;

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

	case ChataigneCommandIDs::showWelcome:
		result.setInfo("Show Welcome Screen...", "", "General", result.readOnlyInKeyEditor);
		break;

	case ChataigneCommandIDs::donate:
		result.setInfo("Be cool and donate", "", "General", result.readOnlyInKeyEditor);
		break;

	case ChataigneCommandIDs::sponsor:
		result.setInfo("Be even cooler and sponsor !", "", "General", result.readOnlyInKeyEditor);
		break;

	case ChataigneCommandIDs::gotoWebsite:
		result.setInfo("Go to website", "", "Help", result.readOnlyInKeyEditor);
		break;
	case ChataigneCommandIDs::gotoDiscord:
		result.setInfo("Go to Discord", "", "Help", result.readOnlyInKeyEditor);
		break;

	case ChataigneCommandIDs::gotoDocs:
		result.setInfo("Go to the Amazing Documentation", "", "Help", result.readOnlyInKeyEditor);
		break;

	case ChataigneCommandIDs::gotoChangelog:
		result.setInfo("See the changelog", "", "Help", result.readOnlyInKeyEditor);
		break;


	case ChataigneCommandIDs::postGithubIssue:
		result.setInfo("Post an issue on github", "", "Help", result.readOnlyInKeyEditor);
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

	case ChataigneCommandIDs::exportSelection:
		result.setInfo("Export Selection", "This will export the current selection as *.lilnut file that can be later imported", "File", result.readOnlyInKeyEditor);
		result.addDefaultKeypress(KeyPress::createFromDescription("s").getKeyCode(), ModifierKeys::altModifier);
		result.setActive(InspectableSelectionManager::mainSelectionManager->currentInspectables.size() > 0);
		break;

	case ChataigneCommandIDs::importSelection:
		result.setInfo("Import...", "This will import a *.lilnut file and add it to the current noisette", "File", result.readOnlyInKeyEditor);
		result.addDefaultKeypress(KeyPress::createFromDescription("o").getKeyCode(), ModifierKeys::altModifier);
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
		ChataigneCommandIDs::showWelcome,
		ChataigneCommandIDs::donate,
		ChataigneCommandIDs::sponsor,
		ChataigneCommandIDs::gotoWebsite,
		ChataigneCommandIDs::gotoDiscord,
		ChataigneCommandIDs::gotoDocs,
		ChataigneCommandIDs::gotoChangelog,
		ChataigneCommandIDs::postGithubIssue,
		ChataigneCommandIDs::importSelection,
		ChataigneCommandIDs::exportSelection,
		ChataigneCommandIDs::goToCommunityModules,
		ChataigneCommandIDs::reloadCustomModules,
		ChataigneCommandIDs::exitGuide,
	};

	commands.addArray(ids, numElementsInArray(ids));
	for (int i = 0; i < Guider::getInstance()->factory.defs.size(); ++i) commands.add(ChataigneCommandIDs::guideStart + i);
}


PopupMenu MainContentComponent::getMenuForIndex(int topLevelMenuIndex, const String& menuName)
{
	PopupMenu menu = OrganicMainContentComponent::getMenuForIndex(topLevelMenuIndex, menuName);

	if (menuName == "Help")
	{
		menu.addCommandItem(&getCommandManager(), ChataigneCommandIDs::showAbout);
		menu.addCommandItem(&getCommandManager(), ChataigneCommandIDs::showWelcome);
		menu.addCommandItem(&getCommandManager(), ChataigneCommandIDs::donate);
		menu.addCommandItem(&getCommandManager(), ChataigneCommandIDs::sponsor);
		menu.addSeparator();
		menu.addCommandItem(&getCommandManager(), ChataigneCommandIDs::gotoWebsite);
		menu.addCommandItem(&getCommandManager(), ChataigneCommandIDs::gotoDiscord);
		menu.addCommandItem(&getCommandManager(), ChataigneCommandIDs::gotoDocs);
		menu.addCommandItem(&getCommandManager(), ChataigneCommandIDs::gotoChangelog);
		menu.addCommandItem(&getCommandManager(), ChataigneCommandIDs::postGithubIssue);

	}
	else if (menuName == "Guides")
	{
		for (int i = 0; i < Guider::getInstance()->factory.defs.size(); ++i)
		{
			menu.addCommandItem(&getCommandManager(), ChataigneCommandIDs::guideStart + i);
		}

		menu.addSeparator();
		menu.addCommandItem(&getCommandManager(), ChataigneCommandIDs::exitGuide);
	}

	return menu;
}

void MainContentComponent::fillFileMenuInternal(PopupMenu& menu)
{
	menu.addCommandItem(&getCommandManager(), ChataigneCommandIDs::importSelection);
	menu.addCommandItem(&getCommandManager(), ChataigneCommandIDs::exportSelection);
	menu.addSeparator();
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
		aboutWindow.reset(new AboutWindow());
		DialogWindow::showDialog("About", aboutWindow.get(), getTopLevelComponent(), Colours::transparentBlack, true);
	}
	break;

	case ChataigneCommandIDs::showWelcome:
	{
		welcomeScreen.reset(new WelcomeScreen());
		DialogWindow::showDialog("Welcome", welcomeScreen.get(), getTopLevelComponent(), Colours::black, true);

	}
	break;

	case ChataigneCommandIDs::donate:
		URL("https://www.paypal.me/benkuper").launchInDefaultBrowser();
		break;

	case ChataigneCommandIDs::sponsor:
		URL("https://github.com/sponsors/benkuper").launchInDefaultBrowser();
		break;

	case ChataigneCommandIDs::gotoWebsite:
		URL("http://benjamin.kuperberg.fr/chataigne").launchInDefaultBrowser();
		break;

	case ChataigneCommandIDs::gotoDiscord:
		URL("https://discord.gg/wYNB3rK").launchInDefaultBrowser();
		break;

	case ChataigneCommandIDs::gotoDocs:
		URL("https://benjamin.kuperberg.fr/chataigne/docs").launchInDefaultBrowser();
		break;

	case ChataigneCommandIDs::gotoChangelog:
		URL("https://benjamin.kuperberg.fr/chataigne/releases/changelog.html").launchInDefaultBrowser();
		break;

	case ChataigneCommandIDs::postGithubIssue:
		URL("http://github.com/benkuper/Chataigne/issues").launchInDefaultBrowser();
		break;


	case ChataigneCommandIDs::goToCommunityModules:
		ShapeShifterManager::getInstance()->showContent(CommunityModulePanel::getPanelName(), StateMachineView::getPanelName());
		break;

	case ChataigneCommandIDs::reloadCustomModules:
		ModuleManager::getInstance()->factory->updateCustomModules();
		break;

	case ChataigneCommandIDs::exitGuide:
		Guider::getInstance()->setCurrentGuide(nullptr);
		break;

	case ChataigneCommandIDs::exportSelection:
	{
		((ChataigneEngine*)Engine::mainEngine)->exportSelection();
	}
	break;

	case ChataigneCommandIDs::importSelection:
	{
		((ChataigneEngine*)Engine::mainEngine)->importSelection();
	}
	break;

	default:
		return OrganicMainContentComponent::perform(info);
	}

	return true;
}

StringArray MainContentComponent::getMenuBarNames()
{
	StringArray names = OrganicMainContentComponent::getMenuBarNames();
	names.add("Guides");
	names.add("Help");
	return names;
}
