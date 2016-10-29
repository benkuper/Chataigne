/*
  ==============================================================================

    MappingOutputUI.h
    Created: 28 Oct 2016 8:12:08pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef MAPPINGOUTPUTUI_H_INCLUDED
#define MAPPINGOUTPUTUI_H_INCLUDED


#include "BaseItemUI.h"
#include "MappingOutput.h"

class MappingOutputUI :
	public BaseItemUI<MappingOutput>
{
public:
	MappingOutputUI(MappingOutput *);
	virtual ~MappingOutputUI();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MappingOutputUI)
};



#endif  // MAPPINGOUTPUTUI_H_INCLUDED
