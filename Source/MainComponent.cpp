/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
#include "Module/ui/ModuleManagerUI.h"
#include "StateMachine/ui/StateMachineView.h"
#include "TimeMachine/ui/TimeMachineView.h"
#include "Module/Routing/ui/ModuleRouterPanelView.h"
#include "StateMachine/ui/StateManagerUI.h"
#include "TimeMachine/ui/SequenceManagerUI.h"
#include "CustomVariables/ui/CVGroupManagerUI.h"

#include "Module/modules/osc/custom/commands/user/model/ui/UserOSCCommandModelManagerUI.h"
#include "Common/Command/Template/ui/CommandTemplateManagerUI.h"

String getAppVersion();

//==============================================================================
MainContentComponent::MainContentComponent()
{

}


MainContentComponent::~MainContentComponent()
{
	
}

void MainContentComponent::init()
{
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Module Router", &ModuleRouterPanelView::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Modules", &ModuleManagerUI::create));

	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Custom Variables", &CVGroupManagerUI::create));

	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Sequences", &SequenceManagerUI::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Sequence Editor", &TimeMachineView::create));

	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("States", &StateManagerUI::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("State Machine", &StateMachineView::create));

	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("OSC Command Models", &UserOSCCommandModelManagerWindow::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Command Templates", &CommandTemplateManagerPanel::create));


	OrganicMainContentComponent::init();
}
