/*
  ==============================================================================

    MappingFilter.h
    Created: 28 Oct 2016 8:08:53pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef MAPPINGFILTER_H_INCLUDED
#define MAPPINGFILTER_H_INCLUDED


#include "BaseItem.h"

class MappingFilter :
	public BaseItem
{
public:
	MappingFilter();
	virtual ~MappingFilter();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MappingFilter)
};




#endif  // MAPPINGFILTER_H_INCLUDED
