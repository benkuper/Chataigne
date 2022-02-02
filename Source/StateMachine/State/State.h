/*
  ==============================================================================

    State.h
    Created: 28 Oct 2016 8:19:10pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ProcessorManager;

class State :
	public BaseItem
{
public:
	State();
	virtual ~State();

	//
	BoolParameter * active;

	enum LoadBehavior { KEEP, ACTIVE, NONACTIVE };
	EnumParameter * loadActivationBehavior;

	BoolParameter* checkTransitionsOnActivate;
	BoolParameter* focusOnLastActionTriggered;

	//Transition
	Array<StateTransition *> inTransitions;
	Array<StateTransition *> outTransitions;

	std::unique_ptr<ProcessorManager> pm;

	void handleLoadActivation();

	void onContainerParameterChangedInternal(Parameter *) override;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;
	void afterLoadJSONDataInternal() override;

	virtual bool paste() override;
	virtual void selectAll(bool addToSelection = false) override;

	virtual String getTypeString() const override { return "State"; }

	class StateListener
	{
	public:
		virtual ~StateListener() {}
		virtual void stateActivationChanged(State*) {}
		virtual void stateStartActivationChanged(State *) {}
	};

	ListenerList<StateListener> stateListeners;
	void addStateListener(StateListener* newListener) { stateListeners.add(newListener); }
	void removeStateListener(StateListener* listener) { stateListeners.remove(listener); }



private:
	WeakReference<State>::Master masterReference;
	friend class WeakReference<State>;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(State)
};