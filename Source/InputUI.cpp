/*
  ==============================================================================

    InputUI.cpp
    Created: 28 Oct 2016 8:22:16pm
    Author:  bkupe

  ==============================================================================
*/

#include "InputUI.h"
#include "TriggerBlinkUI.h"

InputUI::InputUI(Input * input) :
	BaseItemUI<Input>(input)
{
	activityUI = input->activityTrigger->createBlinkUI();
	activityUI->showLabel = false;
	//activityUI->animateIntensity = false;
	addAndMakeVisible(activityUI);
}

InputUI::~InputUI()
{

}

void InputUI::resized()
{
	BaseItemUI::resized();
	Rectangle<int> r = getLocalBounds().withRight(removeBT->getX()-2).reduced(2);
	activityUI->setBounds(r.removeFromRight(headerHeight));
}
