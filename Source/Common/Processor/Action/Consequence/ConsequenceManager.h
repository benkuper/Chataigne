/*
  ==============================================================================

	ConsequenceManager.h
	Created: 28 Oct 2016 8:07:30pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class ConsequenceManager :
	public BaseManager<BaseItem>,
	public MultiplexTarget,
	public Consequence::ConsequenceListener
{
public:
	ConsequenceManager(const String& name = "Consequences", Multiplex* multiplex = nullptr);
	~ConsequenceManager();

	Trigger* triggerPreview;

	FloatParameter* delay;
	FloatParameter* stagger;
	BoolParameter* killDelaysOnTrigger;
	FloatParameter* staggerProgression;

	Factory<BaseItem> factory;

	bool forceDisabled;


	void triggerAll(int multiplexIndex = 0);
	void cancelDelayedConsequences();

	void setForceDisabled(bool value, bool force = false);
	
	void updateKillDelayTrigger();

	void onContainerTriggerTriggered(Trigger*) override;
	void onContainerParameterChanged(Parameter* p) override;


	void addItemInternal(BaseItem*, var data) override;
	void addItemsInternal(Array<BaseItem*>, var data) override;
	void removeItemInternal(BaseItem*) override;
	void removeItemsInternal(Array<BaseItem*>) override;

	class StaggerLauncher :
		public Thread
	{
	public:
		StaggerLauncher(ConsequenceManager* csm, int multiplexIndex);
		~StaggerLauncher();

		ConsequenceManager* csm;
		int multiplexIndex;

		uint32 timeAtRun;
		int triggerIndex;

		void run() override;
	};
	OwnedArray<StaggerLauncher, CriticalSection> staggerLaunchers;

	void launcherTriggered(StaggerLauncher* launcher);
	void launcherFinished(StaggerLauncher* launcher);

	void multiplexPreviewIndexChanged() override;

	InspectableEditor* getEditorInternal(bool isRoot, Array<Inspectable*> inspectables = Array<Inspectable*>()) override;

	class ConsequenceManagerListener
	{
	public:
		virtual ~ConsequenceManagerListener() {}
		virtual void consequenceEnableChanged(Consequence*) {}
		virtual void consequenceValidationChanged(Consequence*) {}
		virtual void consequenceTriggered(Consequence*) {}
	};


	ListenerList<ConsequenceManagerListener> consequenceManagerListeners;
	void addConsequenceManagerListener(ConsequenceManagerListener* newListener) { consequenceManagerListeners.add(newListener); }
	void removeConsequenceManagerListener(ConsequenceManagerListener* listener) { consequenceManagerListeners.remove(listener); }

	DECLARE_ASYNC_EVENT(ConsequenceManager, ConsequenceManager, csm, ENUM_LIST(MULTIPLEX_PREVIEW_CHANGED, STAGGER_CHANGED));

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConsequenceManager)

};