/*
  ==============================================================================

    MappingFilterManager.h
    Created: 28 Oct 2016 8:08:47pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once


class MappingFilterManager :
	public BaseManager<MappingFilter>,
	public MultiplexTarget,
	public MappingFilter::FilterListener
{
public:
	MappingFilterManager(Multiplex * multiplex = nullptr);
	~MappingFilterManager();

	Array<Array<Parameter*>> inputSources;
	Array<Array<Parameter*>> filteredParameters;
	CriticalSection filterLock;

	Factory<MappingFilter> factory;
	bool isRebuilding;

	bool setupSources(Array<Parameter *> sources, int multiplexIndex);
	bool rebuildFilterChain(MappingFilter * afterThisFilter = nullptr, int multiplexIndex = -1, bool rangeOnly = false);
	void notifyNeedsRebuild(MappingFilter* afterThisFilter = nullptr, bool rangeOnly = false);

	WeakReference<MappingFilter> getLastEnabledFilter() { return lastEnabledFilter; }
	Array<Parameter *> getLastFilteredParameters(int multiplexIndex);

	MappingFilter::ProcessResult processFilters(Array<Parameter *> inputs, int multiplexIndex = 0);

	void addItemInternal(MappingFilter * m, var data) override;
	void removeItemInternal(MappingFilter *) override;
	
	void setItemIndex(MappingFilter* item, int index, bool addToUndo = true) override;
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
		virtual void filterManagerNeedsRebuild(MappingFilter* afterThisFilter, bool rangeOnly) {}
		virtual void filterManagerNeedsProcess() {}
	};

	ListenerList<FilterManagerListener, Array<FilterManagerListener*, CriticalSection>> filterManagerListeners;
	void addFilterManagerListener(FilterManagerListener* newListener) { filterManagerListeners.add(newListener); }
	void removeFilterManagerListener(FilterManagerListener* listener) { filterManagerListeners.remove(listener); }


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MappingFilterManager)

protected:
	WeakReference<MappingFilter> lastEnabledFilter;
};