/*
  ==============================================================================

    StateMachineView.h
    Created: 28 Oct 2016 8:27:44pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef STATEMACHINEVIEW_H_INCLUDED
#define STATEMACHINEVIEW_H_INCLUDED

#include "StateManager.h"
#include "StateEditor.h"
#include "BaseManagerUI.h"
#include "Style.h"

class StateMachineView :
	public BaseManagerUI<StateManager,State,StateEditor>
{
public:
	StateMachineView(StateManager * _manager);
	~StateMachineView();


	virtual StateEditor * addItemUI(State *) override;

	void paint(Graphics &g) override;
	void resized() override;
	StateManager * manager;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StateMachineView)
};



#endif  // STATEMACHINEVIEW_H_INCLUDED
