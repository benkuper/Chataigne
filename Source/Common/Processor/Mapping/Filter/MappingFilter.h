/*
  ==============================================================================

	MappingFilter.h
	Created: 28 Oct 2016 8:08:53pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once


#include "Common/ParameterLink/ParameterLink.h"

class MappingFilter :
	public BaseItem,
	public MultiplexTarget,
	public ParamLinkContainer::ParamLinkContainerListener
{
public:
	MappingFilter(const String& name = "MappingFilter", var params = var(), Multiplex* multiplex = nullptr);
	virtual ~MappingFilter();

	enum ProcessResult { CHANGED, UNCHANGED, STOP_HERE };
	ParamLinkContainer filterParams;

	Array<Controllable::Type> filterTypeFilters; //if not empty, this will filter out the parameters passed to the processSingleParameterInternal function

	Array<Array<WeakReference<Parameter>>> sourceParams;
	OwnedArray<OwnedArray<Parameter>> filteredParameters; //not in hierarchy, first dimension is multiplex

	Array<var> previousValues; //for checking, multiplexed

	bool processOnSameValue; //disabling this allows for fast checking and stopping if source and dest values are the same
	bool autoSetRange; //if true, will check at process if ranges are differents between source and filtered, and if so, will reassign

	bool filterParamsAreDirty; //This is use to force processing even if input has not changed when a filterParam has been changed

	virtual bool setupSources(Array<Parameter*> sources, int multiplexIndex, bool rangeOnly = false);
	virtual void setupParametersInternal(int mutiplexIndex, bool rangeOnly = false);
	virtual Parameter* setupSingleParameterInternal(Parameter* source, int multiplexIndex, bool rangeOnly = false);

	ProcessResult process(Array<Parameter*> inputs, int multiplexIndex);
	virtual ProcessResult processInternal(Array<Parameter*> inputs, int multiplexIndex);
	virtual ProcessResult processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex) { return UNCHANGED; }

	virtual void onContainerParameterChangedInternal(Parameter* p) override;
	virtual void onControllableFeedbackUpdateInternal(ControllableContainer*, Controllable* p) override;
	virtual void filterParamChanged(Parameter*) {};

	void linkUpdated(ParamLinkContainer* c, ParameterLink* pLink) override;

	void multiplexPreviewIndexChanged() override;

	virtual void clearItem() override;

	virtual var getJSONData() override;
	virtual void loadJSONDataInternal(var data) override;

	InspectableEditor* getEditorInternal(bool isRoot) override;

	virtual void parameterRangeChanged(Parameter*) override;

	class  FilterListener
	{
	public:
		/** Destructor. */
		virtual ~FilterListener() {}
		virtual void filterStateChanged(MappingFilter*) {}
		virtual void filterNeedsProcess(MappingFilter*) {};
		virtual void filteredParamsChanged(MappingFilter*) {}
		virtual void filteredParamRangeChanged(MappingFilter*) {}
	};

	ListenerList<FilterListener> mappingFilterListeners;
	void addMappingFilterListener(FilterListener* newListener) { mappingFilterListeners.add(newListener); }
	void removeMappingFilterListener(FilterListener* listener) { mappingFilterListeners.remove(listener); }

	class FilterEvent {
	public:
		enum Type { FILTER_REBUILT };
		FilterEvent(Type type, MappingFilter* f) : type(type), filter(f) {}
		Type type;
		MappingFilter* filter;
	};

	QueuedNotifier<FilterEvent> filterAsyncNotifier;
	typedef QueuedNotifier<FilterEvent>::Listener AsyncFilterListener;

	void addAsyncFilterListener(AsyncFilterListener* newListener) { filterAsyncNotifier.addListener(newListener); }
	void addAsyncCoalescedFilterListener(AsyncFilterListener* newListener) { filterAsyncNotifier.addAsyncCoalescedListener(newListener); }
	void removeAsyncFilterListener(AsyncFilterListener* listener) { filterAsyncNotifier.removeListener(listener); }


	virtual String getTypeString() const override { jassert(false); return "[ERROR]"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MappingFilter)

private:
	JUCE_DECLARE_WEAK_REFERENCEABLE(MappingFilter)
};

