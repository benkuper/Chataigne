/*
  ==============================================================================

    ModuleUI.cpp
    Created: 8 Dec 2016 2:36:51pm
    Author:  Ben

  ==============================================================================
*/

#include "ModuleUI.h"
#include "TriggerBlinkUI.h"

ModuleUI::ModuleUI(Module * input) :
	BaseItemUI<Module>(input)
{
	activityUI = input->activityTrigger->createBlinkUI();
	activityUI->showLabel = false;
	//activityUI->animateIntensity = false;
	addAndMakeVisible(activityUI);
}

ModuleUI::~ModuleUI()
{

}

void ModuleUI::resized()
{
	BaseItemUI::resized();
	Rectangle<int> r = getLocalBounds().withRight(removeBT->getX() - 2).reduced(2);
	activityUI->setBounds(r.removeFromRight(headerHeight));
}