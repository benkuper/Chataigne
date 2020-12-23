/*
  ==============================================================================

    ConsequenceManager.h
    Created: 28 Oct 2016 8:07:30pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Consequence.h"
#include "../../Iterator/Iterator.h"

class ConsequenceManager :
	public BaseManager<Consequence>,
	public IterativeTarget,
	public Consequence::ConsequenceListener
{
public:
	ConsequenceManager(const String &name = "Consequences", IteratorProcessor * iterator = nullptr);
	~ConsequenceManager();
	
	FloatParameter * delay;
	FloatParameter * stagger;

	bool forceDisabled;

	Consequence* createItem() override;

	void triggerAll(int iterationIndex = 0);

	void setForceDisabled(bool value, bool force = false);

	void onContainerTriggerTriggered(Trigger *) override;
	void addItemInternal(Consequence *, var data) override;
	void removeItemInternal(Consequence *) override;


	class StaggerLauncher :
		public Thread
	{
	public:
		StaggerLauncher(ConsequenceManager * csm, int iterationIndex);
		~StaggerLauncher();

		ConsequenceManager* csm;
		int iterationIndex;

		uint32 timeAtRun;
		int triggerIndex;

		void run() override;
	};
	OwnedArray<StaggerLauncher> staggerLaunchers;

	void launcherFinished(StaggerLauncher * launcher);

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