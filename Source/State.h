/*
  ==============================================================================

    State.h
    Created: 28 Oct 2016 8:19:10pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED



#include "ProcessorManager.h"
#include "StateTransition.h"

class State :
	public BaseItem
{
public:
	State();
	virtual ~State();

	//
	BoolParameter * active;

	//Transition
	Array<StateTransition *> inTransitions;
	Array<StateTransition *> outTransitions;

	ProcessorManager pm;

	void onContainerParameterChangedInternal(Parameter *) override;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	virtual String getTypeString() const override { return "State"; }

	class StateListener
	{
	public:
		virtual ~StateListener() {}
		virtual void stateActivationChanged(State *) {}
	};

	ListenerList<StateListener> stateListeners;
	void addStateListener(StateListener* newListener) { stateListeners.add(newListener); }
	void removeStateListener(StateListener* listener) { stateListeners.remove(listener); }



private:
	WeakReference<State>::Master masterReference;
	friend class WeakReference<State>;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(State)
};



#endif  // STATE_H_INCLUDED
