/*
  ==============================================================================

    StateManager.h
    Created: 28 Oct 2016 8:19:15pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef STATEMANAGER_H_INCLUDED
#define STATEMANAGER_H_INCLUDED


#include "State.h"
#include "StateTransitionManager.h"
#include "StateModule.h"

class StateManager :
	public BaseManager<State>,
	public State::StateListener
{
public:
	juce_DeclareSingleton(StateManager, false)

	StateManager();
	~StateManager();

	StateModule module;

	StateTransitionManager stm;

	WeakReference<State> activeState;
	void setActiveState(State * s);

	void addItemInternal(State * s, var data) override;
	void removeItemInternal(State * s) override;

	void stateActivationChanged(State * s) override;

	PopupMenu getAllStatesMenu();
	State * getStateForItemID(int itemID);
	PopupMenu getAllActionsMenu();
	Action * getActionForItemID(int itemID);
	PopupMenu getAllMappingsMenu();
	Mapping * getMappingForItemID(int itemID);


	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	InspectableEditor * getEditor(bool isRoot) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StateManager)
};





#endif  // STATEMANAGER_H_INCLUDED
