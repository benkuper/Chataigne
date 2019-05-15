/*
  ==============================================================================

    SequenceTransportUI.h
    Created: 20 Nov 2016 2:24:35pm
    Author:  Ben Kuper

  ==============================================================================
*/

#ifndef SEQUENCETRANSPORTUI_H_INCLUDED
#define SEQUENCETRANSPORTUI_H_INCLUDED

#include "../Sequence.h"

class SequenceTransportUI :
	public Component,
	public Sequence::SequenceListener
{
public:
	SequenceTransportUI(Sequence * _sequence);
	~SequenceTransportUI();

	Sequence * sequence;
	TimeLabel timeLabel;
	
	std::unique_ptr<TriggerImageUI> togglePlayUI;
	std::unique_ptr<TriggerImageUI> stopUI;
	std::unique_ptr<TriggerImageUI> prevCueUI;
	std::unique_ptr<TriggerImageUI> nextCueUI;
	
	void paint(Graphics &g) override;
	void resized() override;

	void sequenceMasterAudioModuleChanged(Sequence *) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceTransportUI)
};


#endif  // SEQUENCETRANSPORTUI_H_INCLUDED
