/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
#include "Module/modules/osc/custom/commands/user/model/ui/UserOSCCommandModelManagerUI.h"
#include "Module/ui/ModuleManagerUI.h"
#include "StateMachine/ui/StateMachineView.h"
#include "TimeMachine/ui/TimeMachineView.h"
#include "Module/Routing/ui/ModuleRouterPanelView.h"
#include "StateMachine/ui/StateManagerUI.h"
#include "TimeMachine/ui/SequenceManagerUI.h"
#include "CustomVariables/ui/CVGroupManagerUI.h"

String getAppVersion();

//==============================================================================
MainContentComponent::MainContentComponent()
{
	
	setSize(800, 600);

	Engine::mainEngine->addEngineListener(this);
	
	lookAndFeelOO = new LookAndFeelOO();
	LookAndFeel::setDefaultLookAndFeel(lookAndFeelOO);

	//ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Help", &HelpPanel::create));

	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Module Router", &ModuleRouterPanelView::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Modules", &ModuleManagerUI::create));
	
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Custom Variables", &CVGroupManagerUI::create));
	
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Sequences", &SequenceManagerUI::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Sequence Editor", &TimeMachineView::create));

	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("States", &StateManagerUI::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("State Machine", &StateMachineView::create));

	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("OSC Command Models", &UserOSCCommandModelManagerWindow::create));

	
	ShapeShifterManager::getInstance()->setDefaultFileData(BinaryData::default_chalayout);
	ShapeShifterManager::getInstance()->setLayoutInformations("chalayout", "Chataigne/layouts");

	String lastVersion = getAppProperties().getCommonSettings(true)->getValue("lastVersion", "0");
	if (lastVersion != getAppVersion())
	{
		ShapeShifterManager::getInstance()->loadDefaultLayoutFile(true);
	} else
	{
		ShapeShifterManager::getInstance()->loadLastSessionLayoutFile();
	}
	
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



void MainContentComponent::startLoadFile()
{

	if (fileProgressWindow != nullptr)
	{
		removeChildComponent(fileProgressWindow);
		fileProgressWindow = nullptr;
	}

	fileProgressWindow = new ProgressWindow("Loading File...", Engine::mainEngine);
	addAndMakeVisible(fileProgressWindow);
	fileProgressWindow->setSize(getWidth(), getHeight());
}

void MainContentComponent::fileProgress(float percent, int state)
{
	if (fileProgressWindow != nullptr)
	{
		fileProgressWindow->setProgress(percent);
	} else
	{
		DBG("Window is null but still got progress");
	}
}

void MainContentComponent::endLoadFile()
{
	if (fileProgressWindow != nullptr)
	{
		removeChildComponent(fileProgressWindow);
		fileProgressWindow = nullptr;
	}
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
	grabKeyboardFocus();
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
