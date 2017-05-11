/*
  ==============================================================================

    SequenceTransportUI.cpp
    Created: 20 Nov 2016 2:24:35pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "SequenceTransportUI.h"
#include "AudioModule.h"
#include "TriggerImageUI.h"

SequenceTransportUI::SequenceTransportUI(Sequence * _sequence) :
	sequence(_sequence),
	timeLabel(_sequence->currentTime)
{
	timeLabel.maxFontHeight = 16;
	addAndMakeVisible(&timeLabel);
	sequence->addSequenceListener(this);
	 
	/*
	togglePlayUI = sequence->togglePlayTrigger->createImageUI(AssetManager::getInstance()->getPlayImage());
	stopUI = sequence->stopTrigger->createImageUI(AssetManager::getInstance()->getStopImage());
	nextCueUI = sequence->nextCue->createImageUI(AssetManager::getInstance()->getNextCueImage());
	prevCueUI = sequence->prevCue->createImageUI(AssetManager::getInstance()->getPrevCueImage());
	*/
	/*
	addAndMakeVisible(togglePlayUI);
	addAndMakeVisible(stopUI);
	addAndMakeVisible(nextCueUI);
	addAndMakeVisible(prevCueUI);
	*/
	
}

SequenceTransportUI::~SequenceTransportUI()
{
	togglePlayUI = nullptr;
	stopUI = nullptr;
	nextCueUI = nullptr;
	prevCueUI = nullptr;

	sequence->removeSequenceListener(this);
}

void SequenceTransportUI::paint(Graphics &g)
{
	if (sequence->timeIsDrivenByAudio())
	{
		g.setColour(AUDIO_COLOR);
		Rectangle<int> r = getLocalBounds().reduced(2).removeFromTop(timeLabel.getHeight()).withLeft(timeLabel.getRight() + 5);
		g.setColour(AUDIO_COLOR.brighter(.7f));
		g.drawFittedText(sequence->masterAudioModule->niceName, r.reduced(2), Justification::centred, 1);
	}
}

void SequenceTransportUI::resized()
{
	Rectangle<int> r = getLocalBounds().reduced(2);
	timeLabel.setBounds(r.removeFromTop(20).removeFromLeft(110));
	r.removeFromTop(2);
	
	
	Rectangle<int> pr = r.removeFromTop(24);
	/*
	togglePlayUI->setBounds(pr.removeFromLeft(pr.getHeight()));
	stopUI->setBounds(pr.removeFromLeft(pr.getHeight()));
	prevCueUI->setBounds(pr.removeFromLeft(pr.getHeight()));
	nextCueUI->setBounds(pr.removeFromLeft(pr.getHeight()));
	*/
}

void SequenceTransportUI::sequenceMasterAudioModuleChanged(Sequence *)
{
	repaint();
}
