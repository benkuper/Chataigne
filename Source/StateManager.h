/*
  ==============================================================================

    StateManager.h
    Created: 28 Oct 2016 8:19:15pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef STATEMANAGER_H_INCLUDED
#define STATEMANAGER_H_INCLUDED

#include "BaseManager.h"
#include "State.h"
#include "StateTransitionManager.h"

class StateManager :
	public BaseManager<State>,
	public State::StateListener
{
public:
	juce_DeclareSingleton(StateManager, false)

	StateManager();
	~StateManager();

	StateTransitionManager stm;

	WeakReference<State> activeState;
	void setActiveState(State * s);

	State * addItem(const Point<float> &initialPosition);
	void addItemInternal(State * s, var data) override;
	void removeItemInternal(State * s) override;

	void stateActivationChanged(State * s) override;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	InspectableEditor * getEditor(bool isRoot) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StateManager)
};





#endif  // STATEMANAGER_H_INCLUDED
