/*
  ==============================================================================

    SequenceLayerManager.h
    Created: 28 Oct 2016 8:15:28pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef SEQUENCELAYERMANAGER_H_INCLUDED
#define SEQUENCELAYERMANAGER_H_INCLUDED


#include "BaseManager.h"
#include "SequenceLayer.h"

class SequenceLayerManager :
	public BaseManager<SequenceLayer>
{
public:
	SequenceLayerManager(Sequence * _sequence);
	~SequenceLayerManager();

	void addItemInternal(SequenceLayer *, var data) override;

	Sequence * sequence;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceLayerManager)

};



#endif  // SEQUENCELAYERMANAGER_H_INCLUDED
