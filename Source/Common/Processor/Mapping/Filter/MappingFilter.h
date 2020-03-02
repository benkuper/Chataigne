/*
  ==============================================================================

    MappingFilter.h
    Created: 28 Oct 2016 8:08:53pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once


#include "JuceHeader.h"

class MappingFilter :
	public BaseItem
{
public:
	MappingFilter(const String &name = "MappingFilter", var params = var());
	virtual ~MappingFilter();

	Array<Parameter *> sourceParams;
	ControllableContainer filterParams;
	
	Array<Controllable::Type> filterTypeFilters; //if not empty, this will filter out the parameters passed to the processSingleParameterInternal function

	OwnedArray<Parameter> filteredParameters; //not in hierarchy

	bool needsContinuousProcess;
	bool autoSetRange; //if true, will check at process if ranges are differents between source and filtered, and if so, will reassign

	bool setupSources(Array<Parameter *> sources);
	virtual bool setupParametersInternal();
	virtual Parameter * setupSingleParameterInternal(Parameter * source);

	bool process();
	virtual bool processInternal();
	virtual void processSingleParameterInternal(Parameter* source, Parameter* out) {}

	virtual void onControllableFeedbackUpdateInternal(ControllableContainer *, Controllable * p) override;
	virtual void filterParamChanged(Parameter * ) {};

	virtual void clearItem() override;

	virtual var getJSONData() override;
	virtual void loadJSONDataInternal(var data) override;

	InspectableEditor * getEditor(bool isRoot) override;

	virtual void parameterRangeChanged(Parameter *) override;

	class  FilterListener
	{
	public:
		/** Destructor. */
		virtual ~FilterListener() {}
		virtual void filterStateChanged(MappingFilter*) {}
		virtual void filterNeedsProcess(MappingFilter *) {};
		virtual void filteredParamRangeChanged(MappingFilter *) {}
	};

	ListenerList<FilterListener> mappingFilterListeners;
	void addMappingFilterListener(FilterListener* newListener) { mappingFilterListeners.add(newListener); }
	void removeMappingFilterListener(FilterListener* listener) { mappingFilterListeners.remove(listener); }

	class FilterEvent {
	public:
		enum Type { FILTER_REBUILT };
		FilterEvent(Type type, MappingFilter* f) : type(type), filter(f) {}
		Type type;
		MappingFilter * filter;
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
