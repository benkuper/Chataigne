/*
  ==============================================================================

    SequenceManager.h
    Created: 28 Oct 2016 8:13:04pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef SEQUENCEMANAGER_H_INCLUDED
#define SEQUENCEMANAGER_H_INCLUDED



#include "Sequence/Sequence.h"
#include "Module/modules/sequence/SequenceModule.h"

class SequenceLayer;
class TimeCue;

class SequenceManager :
	public BaseManager<Sequence>
{
public:
	juce_DeclareSingleton(SequenceManager, false)

	SequenceManager();
	~SequenceManager();

	SequenceModule module;


	static Sequence * showMenuAndGetSequence();
	Sequence * getSequenceForItemID(int itemID);

	static SequenceLayer * showmMenuAndGetLayer();
	SequenceLayer * getLayerForItemID(int itemID);

	static TimeCue * showMenuAndGetCue();
	TimeCue * getCueForItemID(int itemID);


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceManager)
};





#endif  // SEQUENCEMANAGER_H_INCLUDED
