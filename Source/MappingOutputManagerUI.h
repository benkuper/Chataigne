/*
  ==============================================================================

    MappingOutputManagerUI.h
    Created: 28 Oct 2016 8:12:18pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef MAPPINGOUTPUTMANAGERUI_H_INCLUDED
#define MAPPINGOUTPUTMANAGERUI_H_INCLUDED



#include "BaseManagerUI.h"
#include "MappingOutputUI.h"
#include "MappingOutputManager.h"

class MappingOutputManagerUI :
	public BaseManagerUI<MappingOutputManager, MappingOutput, MappingOutputUI>
{
public:

	MappingOutputManagerUI(MappingOutputManager *_manager);
	~MappingOutputManagerUI();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MappingOutputManagerUI)
};




#endif  // MAPPINGOUTPUTMANAGERUI_H_INCLUDED
