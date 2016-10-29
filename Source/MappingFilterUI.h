/*
  ==============================================================================

    MappingFilterUI.h
    Created: 28 Oct 2016 8:09:06pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef MAPPINGFILTERUI_H_INCLUDED
#define MAPPINGFILTERUI_H_INCLUDED


#include "BaseItemUI.h"
#include "MappingFilter.h"

class MappingFilterUI :
	public BaseItemUI<MappingFilter>
{
public:
	MappingFilterUI(MappingFilter *);
	virtual ~MappingFilterUI();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MappingFilterUI)
};




#endif  // MAPPINGFILTERUI_H_INCLUDED
