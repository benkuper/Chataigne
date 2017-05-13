/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
#include "UserOSCCommandModelManagerUI.h"
#include "ModuleManagerUI.h"
#include "StateMachineView.h"
#include "TimeMachineView.h"
#include "UserOSCCommandModelManagerUI.h"
#include "ModuleRouterPanelView.h"
#include "StateManagerUI.h"
#include "SequenceManagerUI.h"

//==============================================================================
MainContentComponent::MainContentComponent()
{
	
	setSize(800, 600);

	Engine::mainEngine->addEngineListener(this);
	
	lookAndFeelOO = new LookAndFeelOO();
	LookAndFeel::setDefaultLookAndFeel(lookAndFeelOO);


	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Modules", &ModuleManagerUI::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("State Machine", &StateMachineView::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Time Machine", &TimeMachineView::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("OSC Command Models", &UserOSCCommandModelManagerWindow::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Module Router", &ModuleRouterPanelView::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("States", &StateManagerUI::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Sequences", &SequenceManagerUI::create));
	
	ShapeShifterManager::getInstance()->setDefaultFileData(BinaryData::default_chalayout);
	ShapeShifterManager::getInstance()->setLayoutInformations("chalayout", "Chataigne/layouts");
	ShapeShifterManager::getInstance()->loadLastSessionLayoutFile();
	
	(&getCommandManager())->registerAllCommandsForTarget(this);
	(&getCommandManager())->setFirstCommandTarget(this);
	
	(&getCommandManager())->getKeyMappings()->resetToDefaultMappings();
	addKeyListener((&getCommandManager())->getKeyMappings());
	

#if JUCE_MAC
	setMacMainMenu(this, nullptr, "");
#else
	//setMenu (this); //done in Main.cpp as it's a method of DocumentWindow
#endif



}

MainContentComponent::~MainContentComponent()
{
#if JUCE_MAC
    setMacMainMenu(nullptr,nullptr,"");
#endif

	if(Engine::mainEngine != nullptr) Engine::mainEngine->removeEngineListener(this);
	ShapeShifterManager::deleteInstance();
	
}

void MainContentComponent::init()
{
	addAndMakeVisible(&ShapeShifterManager::getInstance()->mainContainer);
}

void MainContentComponent::paint (Graphics& g)
{
    g.fillAll (BG_COLOR.darker());
}

void MainContentComponent::resized()
{
	Rectangle<int> r = getLocalBounds();
	ShapeShifterManager::getInstance()->mainContainer.setBounds(r);
}
