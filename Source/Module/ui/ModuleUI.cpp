/*
  ==============================================================================

    ModuleUI.cpp
    Created: 8 Dec 2016 2:36:51pm
    Author:  Ben

  ==============================================================================
*/

#include "ModuleUI.h"
#include "UI/ChataigneAssetManager.h"

ModuleUI::ModuleUI(Module * module) :
	BaseItemUI<Module>(module)
{
	headerHeight = 20;

	module->addModuleListener(this);

	inActivityUI.reset(module->inActivityTrigger->createImageUI(ChataigneAssetManager::getInstance()->getInImage()));
	inActivityUI->showLabel = false;
	addAndMakeVisible(inActivityUI.get());
	inActivityUI->setVisible(module->hasInput);

	outActivityUI.reset(module->outActivityTrigger->createImageUI(ChataigneAssetManager::getInstance()->getOutImage()));
	outActivityUI->showLabel = false;
	addAndMakeVisible(outActivityUI.get());
	outActivityUI->setVisible(module->hasOutput);

	if (module->connectionFeedbackRef != nullptr)
	{
		connectionFeedbackUI.reset(module->connectionFeedbackRef->createImageToggle(ChataigneAssetManager::getInstance()->getBTDoubleImage(ChataigneAssetManager::getInstance()->getConnectedImage(), ChataigneAssetManager::getInstance()->getDisconnectedImage())));
		addAndMakeVisible(connectionFeedbackUI.get());
	}
}

ModuleUI::~ModuleUI()
{
	item->removeModuleListener(this);

}

void ModuleUI::resizedInternalHeader(Rectangle<int>& r)
{
	outActivityUI->setBounds(r.removeFromRight(r.getHeight()));
	inActivityUI->setBounds(r.removeFromRight(r.getHeight()));
	if (connectionFeedbackUI != nullptr) connectionFeedbackUI->setBounds(r.removeFromRight(r.getHeight()));
}

void ModuleUI::moduleIOConfigurationChanged()
{
	inActivityUI->setVisible(item->hasInput);
	outActivityUI->setVisible(item->hasOutput);
}
