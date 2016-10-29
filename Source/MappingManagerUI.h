/*
  ==============================================================================

    MappingManagerUI.h
    Created: 28 Oct 2016 8:06:06pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef MAPPINGMANAGERUI_H_INCLUDED
#define MAPPINGMANAGERUI_H_INCLUDED



#include "BaseManagerUI.h"
#include "MappingUI.h"
#include "MappingManager.h"

class MappingManagerUI :
	public BaseManagerUI<MappingManager, Mapping, MappingUI>
{
public:

	MappingManagerUI(MappingManager *_manager);
	~MappingManagerUI();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MappingManagerUI)
};




#endif  // MAPPINGMANAGERUI_H_INCLUDED
