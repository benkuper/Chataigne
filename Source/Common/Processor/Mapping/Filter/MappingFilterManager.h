/*
  ==============================================================================

    MappingFilterManager.h
    Created: 28 Oct 2016 8:08:47pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef MAPPINGFILTERMANAGER_H_INCLUDED
#define MAPPINGFILTERMANAGER_H_INCLUDED


#include "MappingFilter.h"

class MappingFilterManager :
	public BaseManager<MappingFilter>
{
public:
	MappingFilterManager();
	~MappingFilterManager();

	Parameter * inputSourceParam;
	void setupSource(Parameter * source);
	void rebuildFilterChain();
	
	Parameter * processFilters();
	Factory<MappingFilter> factory;

	void addItemInternal(MappingFilter * m, var data) override;
	void removeItemInternal(MappingFilter *) override;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MappingFilterManager)
};



#endif  // MAPPINGFILTERMANAGER_H_INCLUDED
