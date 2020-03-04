/*
  ==============================================================================

    MappingFilterManager.h
    Created: 28 Oct 2016 8:08:47pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "MappingFilter.h"

class MappingFilterManager :
	public BaseManager<MappingFilter>,
	public MappingFilter::FilterListener
{
public:
	MappingFilterManager();
	~MappingFilterManager();

	Array<Parameter *> inputSourceParams;

	bool setupSources(Array<Parameter *> sources);
	bool rebuildFilterChain(MappingFilter * afterThisFilter = nullptr);

	void notifyNeedsRebuild(MappingFilter * afterThisFilter = nullptr);

	WeakReference<MappingFilter> getLastEnabledFilter() { return lastEnabledFilter; }
	Array<Parameter *> getLastFilteredParameters();

	Array<Parameter *> processFilters();
	Factory<MappingFilter> factory;

	void addItemInternal(MappingFilter * m, var data) override;
	void removeItemInternal(MappingFilter *) override;
	
	void setItemIndex(MappingFilter* item, int index) override;
	void reorderItems() override;
	
	void filterStateChanged(MappingFilter* mf) override;
	void filterNeedsProcess(MappingFilter* mf) override;
	void filteredParamsChanged(MappingFilter* mf) override;
	void filteredParamRangeChanged(MappingFilter* mf) override;

	void loadJSONDataManagerInternal(var data) override;

	class  FilterManagerListener
	{
	public:
		/** Destructor. */
		virtual ~FilterManagerListener() {}
		virtual void filterManagerNeedsRebuild(MappingFilter* afterThisFilter) {}
		virtual void filterManagerNeedsProcess() {}
	};

	ListenerList<FilterManagerListener> filterManagerListeners;
	void addFilterManagerListener(FilterManagerListener* newListener) { filterManagerListeners.add(newListener); }
	void removeFilterManagerListener(FilterManagerListener* listener) { filterManagerListeners.remove(listener); }


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MappingFilterManager)

protected:
	WeakReference<MappingFilter> lastEnabledFilter;
};