/*
  ==============================================================================

    MappingFilterManagerUI.h
    Created: 28 Oct 2016 8:09:00pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef MAPPINGFILTERMANAGERUI_H_INCLUDED
#define MAPPINGFILTERMANAGERUI_H_INCLUDED


#include "BaseManagerUI.h"
#include "MappingFilterUI.h"
#include "MappingFilterManager.h"

class MappingFilterManagerUI :
	public BaseManagerUI<MappingFilterManager, MappingFilter, MappingFilterUI>
{
public:

	MappingFilterManagerUI(MappingFilterManager *_manager);
	~MappingFilterManagerUI();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MappingFilterManagerUI)
};




#endif  // MAPPINGFILTERMANAGERUI_H_INCLUDED
