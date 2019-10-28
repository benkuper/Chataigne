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

	WeakReference<Parameter> sourceParam;
	WeakReference<Parameter> filteredParameter;
	ControllableContainer filterParams;
	
	String forceOutParameterType;

	bool needsContinuousProcess;
	bool autoSetRange; //if true, will check at process if ranges are differents between source and filtered, and if so, will reassign

	void setupSource(Parameter * source);
	virtual Parameter * setupParameterInternal(Parameter * source);

	void onControllableFeedbackUpdateInternal(ControllableContainer *, Controllable * p) override;
	void onContainerParameterChangedInternal(Parameter *p) override;
	virtual void filterParamChanged(Parameter * ) {};

	Parameter * process(Parameter * source);
	virtual void processInternal() {}

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
		virtual void filteredParamChanged(MappingFilter *) {};
		virtual void filteredParamRangeChanged(MappingFilter *) {}
	};

	ListenerList<FilterListener> mappingFilterListeners;
	void addMappingFilterListener(FilterListener* newListener) { mappingFilterListeners.add(newListener); }
	void removeMappingFilterListener(FilterListener* listener) { mappingFilterListeners.remove(listener); }

	class FilterEvent {
	public:
		enum Type { FILTER_PARAM_CHANGED, FILTER_STATE_CHANGED };
		FilterEvent(Type type, MappingFilter * i) : type(type), filter(i) {}
		Type type;
		MappingFilter * filter;
	};
	QueuedNotifier<FilterEvent> mappingFilterAsyncNotifier;
	typedef QueuedNotifier<FilterEvent>::Listener AsyncListener;


	void addAsyncFilterListener(AsyncListener* newListener) { mappingFilterAsyncNotifier.addListener(newListener); }
	void addAsyncCoalescedFilterListener(AsyncListener* newListener) { mappingFilterAsyncNotifier.addAsyncCoalescedListener(newListener); }
	void removeAsyncFilterListener(AsyncListener* listener) { mappingFilterAsyncNotifier.removeListener(listener); }


	virtual String getTypeString() const override { jassert(false); return "[ERROR]"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MappingFilter)

private:
	JUCE_DECLARE_WEAK_REFERENCEABLE(MappingFilter)
};
