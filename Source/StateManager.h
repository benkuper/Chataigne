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
#include "Mapping.h"

class StateManager :
	public BaseManager<State>,
	public State::StateListener,
	public StateTransitionManager::Listener
{
public:
	juce_DeclareSingleton(StateManager, false)

	StateManager();
	~StateManager();

	StateModule module;

	StateTransitionManager stm;

	BoolParameter * onlyOneActiveState; //whether activating a state automatically deactivates all other non-permanent states

	void setStateActive(State * s);

	void addItemInternal(State * s, var data) override;
	void removeItemInternal(State * s) override;

	void stateActivationChanged(State * s) override;
	void itemAdded(StateTransition * s) override;

	static State * showMenuAndGetState();
	State * getStateForItemID(int itemID);
	static Action * showMenuAndGetAction();
	Action * getActionForItemID(int itemID);
	static Mapping * showMenuAndGetMapping();
	Mapping * getMappingForItemID(int itemID);


	Array<State *> getLinkedStates(State * s, Array<State *> * statesToAvoid = nullptr);

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StateManager)
};





#endif  // STATEMANAGER_H_INCLUDED
