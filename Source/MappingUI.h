/*
  ==============================================================================

    MappingUI.h
    Created: 28 Oct 2016 8:06:18pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef MAPPINGUI_H_INCLUDED
#define MAPPINGUI_H_INCLUDED

#include "Mapping.h"

class MappingUI :
	public BaseItemUI<Mapping>
{
public:
	MappingUI(Mapping *);
	virtual ~MappingUI();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MappingUI)
};




#endif  // MAPPINGUI_H_INCLUDED
