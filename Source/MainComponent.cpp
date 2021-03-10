/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

#include "Module/ModuleIncludes.h"
#include "StateMachine/StateMachineIncludes.h"
#include "TimeMachine/TimeMachineIncludes.h"
#include "CustomVariables/CustomVariablesIncludes.h"

#include "Common/Command/Template/ui/CommandTemplateManagerUI.h"

#include "UI/WelcomeScreen.h"

String getAppVersion();

//==============================================================================
MainContentComponent::MainContentComponent()
{
	getCommandManager().registerAllCommandsForTarget(this);
}


MainContentComponent::~MainContentComponent()
{
	
}

void MainContentComponent::init()
{
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Module Router", &ModuleRouterPanelView::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Modules", &ModuleManagerUI::create));

	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Custom Variables", &CVGroupManagerUI::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Morpher", &MorpherPanel::create));

	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Sequences", &createSequenceManagerUI));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Sequence Editor", &TimeMachineView::create));

	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("States", &StateManagerUI::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("State Machine", &StateMachineView::create));

	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Command Templates", &CommandTemplateManagerPanel::create));


	OrganicMainContentComponent::init();

	String lastVersion = getAppProperties().getUserSettings()->getValue("lastVersion", "");

	if (lastVersion != getAppVersion())
	{
		WelcomeScreen w;
		DialogWindow::showModalDialog("Welcome", &w, getTopLevelComponent(), Colours::black, true);
	}


}
SequenceManagerUI* MainContentComponent::createSequenceManagerUI(const String& name) 
{
	return SequenceManagerUI::create(name, ChataigneSequenceManager::getInstance());
}
