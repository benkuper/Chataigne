/*
  ==============================================================================

    StateManager.h
    Created: 28 Oct 2016 8:19:15pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once


//class Action;
//class Mapping;
//class StandardCondition;
//class StateModule;
//class ProcessorManager;
//class Processor;

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

	std::unique_ptr<StateModule> module;

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

	static void showMenuAndGetState(ControllableContainer* startFromCC, std::function<void(State *)> returnFunc);
	static void showMenuAndGetAction(ControllableContainer* startFromCC, std::function<void(Action*)> returnFunc);
	static void showMenuAndGetMapping(ControllableContainer* startFromCC, std::function<void(Mapping*)> returnFunc);
	static void showMenuAndGetConductor(ControllableContainer* startFromCC, std::function<void(Mapping*)> returnFunc);
	static PopupMenu getProcessorMenuForManager(ProcessorManager* manager, Processor::ProcessorType type, Array<Processor*> *arrayToFill);

	static void showMenuAndGetToggleCondition(ControllableContainer* startFromCC, std::function<void(StandardCondition*)> returnFunc);
	static PopupMenu getToggleConditionMenuForConditionManager(ConditionManager * a, Array<StandardCondition *> * arrayToFill);

	Array<State *> getLinkedStates(State * s, Array<State *> * statesToAvoid = nullptr);

	static var addTransitionFromScript(const var::NativeFunctionArgs& a);

	var getJSONData() override;
	void loadJSONDataManagerInternal(var data) override;

	void endLoadFile() override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StateManager)
};