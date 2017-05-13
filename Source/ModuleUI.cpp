/*
  ==============================================================================

    ModuleUI.cpp
    Created: 8 Dec 2016 2:36:51pm
    Author:  Ben

  ==============================================================================
*/

#include "ModuleUI.h"
#include "ChataigneAssetManager.h"

ModuleUI::ModuleUI(Module * input) :
	BaseItemUI<Module>(input)
{
	inActivityUI = input->inActivityTrigger->createImageUI(ChataigneAssetManager::getInstance()->getInImage());
	inActivityUI->showLabel = false;
	addAndMakeVisible(inActivityUI);

	outActivityUI = input->outActivityTrigger->createImageUI(ChataigneAssetManager::getInstance()->getOutImage());
	outActivityUI->showLabel = false;
	addAndMakeVisible(outActivityUI);
}

ModuleUI::~ModuleUI()
{

}

void ModuleUI::resizedInternalHeader(Rectangle<int>& r)
{
	outActivityUI->setBounds(r.removeFromRight(headerHeight));
	inActivityUI->setBounds(r.removeFromRight(headerHeight));
}