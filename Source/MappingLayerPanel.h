/*
  ==============================================================================

    MappingLayerPanel.h
    Created: 20 Nov 2016 3:07:42pm
    Author:  Ben Kuper

  ==============================================================================
*/

#ifndef MAPPINGLAYERPANEL_H_INCLUDED
#define MAPPINGLAYERPANEL_H_INCLUDED


#include "SequenceLayerPanel.h"
#include "MappingLayer.h"

class MappingLayerPanel :
	public SequenceLayerPanel
{
public:
	MappingLayerPanel(MappingLayer * layer);
	~MappingLayerPanel();

	MappingLayer * mappingLayer;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MappingLayerPanel)
};




#endif  // MAPPINGLAYERPANEL_H_INCLUDED
