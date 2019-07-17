/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
#include "Module/ui/ModuleManagerUI.h"
#include "StateMachine/ui/StateMachineView.h"
#include "Module/Routing/ui/ModuleRouterPanelView.h"
#include "StateMachine/ui/StateManagerUI.h"
#include "CustomVariables/ui/CVGroupManagerUI.h"
#include "CustomVariables/Preset/Morpher/ui/MorpherViewUI.h"
#include "Common/Command/Template/ui/CommandTemplateManagerUI.h"
#include "TimeMachine/ChataigneSequenceManager.h"

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


}
SequenceManagerUI* MainContentComponent::createSequenceManagerUI(const String& name) 
{
	return SequenceManagerUI::create(name, ChataigneSequenceManager::getInstance());
}
