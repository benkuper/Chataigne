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
	public MappingFilter::AsyncListener
{
public:
	MappingFilterManager();
	~MappingFilterManager();

	Array<WeakReference<Parameter>> inputSourceParams;
	void setupSources(Array<WeakReference<Parameter>> sources);
	void rebuildFilterChain();
	WeakReference<MappingFilter> getLastEnabledFilter() { return lastEnabledFilter; }

	Array<WeakReference<Parameter>> processFilters();
	Factory<MappingFilter> factory;

	void addItemInternal(MappingFilter * m, var data) override;
	void removeItemInternal(MappingFilter *) override;
	
	void setItemIndex(MappingFilter* item, int index) override;
	void reorderItems() override;

	void newMessage(const MappingFilter::FilterEvent &e) override;

	void loadJSONDataManagerInternal(var data) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MappingFilterManager)

protected:
	WeakReference<MappingFilter> lastEnabledFilter;
};