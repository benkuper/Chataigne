/*
  ==============================================================================

    ConsequenceManager.h
    Created: 28 Oct 2016 8:07:30pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Consequence.h"

class ConsequenceManager :
	public BaseManager<Consequence>,
	public Consequence::ConsequenceListener,
	public Thread
{
public:
	juce_DeclareSingleton(ConsequenceManager, true)

	ConsequenceManager(const String &name = "Consequences");
	~ConsequenceManager();
	
	Array<Trigger *> triggerAlls;
	FloatParameter * delay;
	FloatParameter * stagger;

	bool forceDisabled;

	//delay and stagger
	uint32 timeAtRun;
	int triggerIndex;

	//iteration
	int iterationCount;
	void setIterationCount(int count);

	void setForceDisabled(bool value, bool force = false);

	void onContainerTriggerTriggered(Trigger *) override;
	void addItemInternal(Consequence *, var data) override;
	void removeItemInternal(Consequence *) override;

	void run() override;

	InspectableEditor * getEditor(bool isRoot) override; 

	class ConsequenceManagerListener
	{
	public:
		virtual ~ConsequenceManagerListener() {}
		virtual void consequenceEnableChanged(Consequence *) {}
		virtual void consequenceValidationChanged(Consequence *) {}
		virtual void consequenceTriggered(Consequence *) {}
	};


	ListenerList<ConsequenceManagerListener> consequenceManagerListeners;
	void addConsequenceManagerListener(ConsequenceManagerListener* newListener) { consequenceManagerListeners.add(newListener); }
	void removeConsequenceManagerListener(ConsequenceManagerListener* listener) { consequenceManagerListeners.remove(listener); }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConsequenceManager)

};