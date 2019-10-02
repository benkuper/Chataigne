/*
  ==============================================================================

    StateUI.cpp
    Created: 28 Oct 2016 8:19:29pm
    Author:  bkupe

  ==============================================================================
*/

#include "StateUI.h"
#include "StateMachine/ui/StateMachineView.h"

StateUI::StateUI(State * state) :
	BaseItemUI<State>(state)
{
	activeUI.reset(item->active->createToggle());
	showInViewBT.reset(AssetManager::getInstance()->getFileBT());

	showInViewBT->addListener(this);

	addAndMakeVisible(activeUI.get());
	addAndMakeVisible(showInViewBT.get());
}

StateUI::~StateUI()
{
}

void StateUI::resizedInternalHeader(Rectangle<int>& r)
{
	activeUI->setBounds(r.removeFromRight(50));
	r.removeFromRight(8);
	showInViewBT->setBounds(r.removeFromRight(r.getHeight()));
	r.removeFromRight(4);

	BaseItemUI::resizedInternalHeader(r);
}

void StateUI::buttonClicked(Button * b)
{
	BaseItemUI::buttonClicked(b);
	if (b == showInViewBT.get())
	{
		StateMachineView * smv = ShapeShifterManager::getInstance()->getContentForType<StateMachineView>();
		if (smv != nullptr)
		{
			StateViewUI* svui = smv->getUIForItem(item);
			smv->frameView(svui);
		}
	}

}
