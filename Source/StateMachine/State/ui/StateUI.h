/*
  ==============================================================================

    StateUI.h
    Created: 28 Oct 2016 8:19:29pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class StateUI :
	public BaseItemUI<State>
{
public:
	StateUI(State * output);
	virtual ~StateUI();

	std::unique_ptr<BoolToggleUI> activeUI;
	std::unique_ptr<ImageButton> showInViewBT;

	void resizedInternalHeader(Rectangle<int>& r) override;

	void buttonClicked(Button* b) override;

	JUCE_DECLARE_NON_COPYABLE(StateUI)
};
