/*
  ==============================================================================

	ConsequenceManager.h
	Created: 28 Oct 2016 8:07:30pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class ConsequenceManager;

class ConsequenceStaggerLauncher :
	public Thread
{
public:
	juce_DeclareSingleton(ConsequenceStaggerLauncher, false)

	ConsequenceStaggerLauncher();
	~ConsequenceStaggerLauncher();


	struct Launch
	{
		Launch(ConsequenceManager* c, int multiplexIndex) : manager(c), startTime(Time::getMillisecondCounter()), multiplexIndex(multiplexIndex), triggerIndex(0) {}


		ConsequenceManager* manager;
		uint32 startTime;
		int multiplexIndex;
		int triggerIndex;
		Launch* currentProcessingLaunch;

		bool isFinished();
	};

	OwnedArray<Launch, juce::CriticalSection> launches;
	Array<Launch*, juce::CriticalSection> toAdd;
	Array<Launch*, juce::CriticalSection> toRemove;

	void run() override;
	void processLaunch(Launch* l);

	void addLaunch(ConsequenceManager* c, int multiplexIndex);
	void removeLaunchesFor(ConsequenceManager* manager, int multiplexIndex);
};

class ConsequenceManager :
	public Manager<BaseItem>,
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
	void cancelDelayedConsequences(int multiplexIndex = 0);

	void setForceDisabled(bool value, bool force = false);

	void updateKillDelayTrigger();

	void onContainerTriggerTriggered(Trigger*) override;
	void onContainerParameterChanged(Parameter* p) override;


	void addItemInternal(BaseItem*, var data) override;
	void addItemsInternal(Array<BaseItem*>, var data) override;
	void removeItemInternal(BaseItem*) override;
	void removeItemsInternal(Array<BaseItem*>) override;

	void launcherTriggered(int multiplexIndex, int triggerIndex);

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


	DECLARE_INSPECTACLE_SAFE_LISTENER(ConsequenceManager, consequenceManager);

	DECLARE_ASYNC_EVENT(ConsequenceManager, ConsequenceManager, csm, ENUM_LIST(MULTIPLEX_PREVIEW_CHANGED, STAGGER_CHANGED), EVENT_INSPECTABLE_CHECK);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConsequenceManager)
};