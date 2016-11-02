/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"



//==============================================================================
MainContentComponent::MainContentComponent()
{
	Engine::getInstance()->addEngineListener(this);

	setSize(800, 600);
	
	LookAndFeel::setDefaultLookAndFeel(lookAndFeelOO = new LookAndFeelOO);
	
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

	if(Engine::getInstanceWithoutCreating() != nullptr) Engine::getInstanceWithoutCreating()->removeEngineListener(this);
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
