/*
  ==============================================================================

    StateManager.h
    Created: 28 Oct 2016 8:19:15pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "State/State.h"
#include "Transition/StateTransitionManager.h"
#include "Module/modules/state/StateModule.h"

class Action;
class Mapping;
class StandardCondition;

class StateManager :
	public BaseManager<State>,
	public State::StateListener,
	public StateTransitionManager::ManagerListener,
	public EngineListener
{
public:
	juce_DeclareSingleton(StateManager, false)

	StateManager();
	~StateManager();

	StateModule module;

	StateTransitionManager stm;

	CommentManager commentManager;

	void clear() override;

	void setStateActive(State * s);

	void addItemInternal(State * s, var data) override;
	void removeItemInternal(State * s) override;

	Array<UndoableAction *> getRemoveItemUndoableAction(State * item) override;
	Array<UndoableAction *> getRemoveItemsUndoableAction(Array<State *> itemsToRemove) override;

	void stateActivationChanged(State* s) override;
	void stateStartActivationChanged(State * s) override;

	void checkStartActivationOverlap(State* s, Array<State *> statesToAvoid = Array<State *>());

	void itemAdded(StateTransition* s) override;
	void itemsAdded(Array<StateTransition *> s) override;
	void itemRemoved(StateTransition* s) override;
	void itemsRemoved(Array<StateTransition*> s) override;

	static State * showMenuAndGetState();
	static Action * showMenuAndGetAction();
	static Mapping * showMenuAndGetMapping();
	static StandardCondition * showMenuAndGetToggleCondition();


	Array<State *> getLinkedStates(State * s, Array<State *> * statesToAvoid = nullptr);

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	void endLoadFile() override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StateManager)
};