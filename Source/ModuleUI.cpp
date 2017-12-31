/*
  ==============================================================================

    ModuleUI.cpp
    Created: 8 Dec 2016 2:36:51pm
    Author:  Ben

  ==============================================================================
*/

#include "ModuleUI.h"
#include "ChataigneAssetManager.h"

ModuleUI::ModuleUI(Module * module) :
	BaseItemUI<Module>(module)
{
	module->addModuleListener(this);

	inActivityUI = module->inActivityTrigger->createImageUI(ChataigneAssetManager::getInstance()->getInImage());
	inActivityUI->showLabel = false;
	addAndMakeVisible(inActivityUI);
	inActivityUI->setVisible(module->hasInput);

	outActivityUI = module->outActivityTrigger->createImageUI(ChataigneAssetManager::getInstance()->getOutImage());
	outActivityUI->showLabel = false;
	addAndMakeVisible(outActivityUI);
	outActivityUI->setVisible(module->hasOutput);
}

ModuleUI::~ModuleUI()
{
	item->removeModuleListener(this);

}

void ModuleUI::resizedInternalHeader(Rectangle<int>& r)
{
	outActivityUI->setBounds(r.removeFromRight(headerHeight));
	inActivityUI->setBounds(r.removeFromRight(headerHeight));
}

void ModuleUI::moduleIOConfigurationChanged()
{
	inActivityUI->setVisible(item->hasInput);
	outActivityUI->setVisible(item->hasOutput);
}
