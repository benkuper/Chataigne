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
#include "AudioLayer.h"

class SequenceLayerManager :
	public BaseManager<SequenceLayer>,
	public AudioLayer::AudioLayerListener //for sequence driven by audio
{
public:
	SequenceLayerManager(Sequence * _sequence);
	~SequenceLayerManager();

	Sequence * sequence;
	AudioLayer * masterAudioLayer;
	
	SequenceLayer * createItem() override;
	void addItemFromData(var data, bool fromUndoableAction = false) override;

	void addItemInternal(SequenceLayer * item, var data) override;
	void removeItemInternal(SequenceLayer * item) override;

	void targetAudioModuleChanged(AudioLayer * layer) override;
	void updateTargetAudioLayer(AudioLayer * excludeLayer = nullptr);
	void setMasterAudioLayer(AudioLayer * layer);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceLayerManager)

};



#endif  // SEQUENCELAYERMANAGER_H_INCLUDED
