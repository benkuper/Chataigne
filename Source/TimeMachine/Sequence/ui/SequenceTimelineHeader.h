/*
  ==============================================================================

    SequenceTimelineHeader.h
    Created: 23 Nov 2016 11:32:15pm
    Author:  Ben Kuper

  ==============================================================================
*/

#ifndef SEQUENCETIMELINEHEADER_H_INCLUDED
#define SEQUENCETIMELINEHEADER_H_INCLUDED

#include "../Sequence.h"
#include "../Cue/ui/TimeCueManagerUI.h"

class TimeNeedleUI :
	public Component
{
public:
	TimeNeedleUI() {}
	~TimeNeedleUI() {}

	void paint(Graphics &g) override;
};

class SequenceTimelineHeader :
	public Component,
	public ContainerAsyncListener
{
public:
	SequenceTimelineHeader(Sequence * _sequence);
	~SequenceTimelineHeader();

	Sequence * sequence;

	TimeNeedleUI needle;

	TimeCueManagerUI cueManagerUI;

	//interaction

	void paint(Graphics &g) override;
	void resized() override;

	void updateNeedlePosition();

	void mouseDown(const MouseEvent &e) override;
	void mouseDrag(const MouseEvent &e) override;
	void mouseDoubleClick(const MouseEvent &e) override;

	int getXForTime(float time);
	float getTimeForX(int tx);

	void newMessage(const ContainerAsyncEvent &e) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceTimelineHeader)

};





#endif  // SEQUENCETIMELINEHEADER_H_INCLUDED
