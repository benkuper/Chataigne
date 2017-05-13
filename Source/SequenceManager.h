/*
  ==============================================================================

    SequenceManager.h
    Created: 28 Oct 2016 8:13:04pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef SEQUENCEMANAGER_H_INCLUDED
#define SEQUENCEMANAGER_H_INCLUDED



#include "Sequence.h"
#include "SequenceModule.h"

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


	PopupMenu getAllSequencesMenu();
	Sequence * getSequenceForItemID(int itemID);

	PopupMenu getAllLayersMenu();
	SequenceLayer * getLayerForItemID(int itemID);

	PopupMenu getAllCuesMenu();
	TimeCue * getCueForItemID(int itemID);


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceManager)
};





#endif  // SEQUENCEMANAGER_H_INCLUDED
