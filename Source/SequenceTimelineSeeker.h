/*
  ==============================================================================

    SequenceTimelineSeeker.h
    Created: 23 Nov 2016 11:31:50pm
    Author:  Ben Kuper

  ==============================================================================
*/

#ifndef SEQUENCETIMELINESEEKER_H_INCLUDED
#define SEQUENCETIMELINESEEKER_H_INCLUDED

#include "Sequence.h"

class SeekHandle :
	public Component
{
public:
	SeekHandle() {}
	~SeekHandle() {}

	void paint(Graphics &g) override;
	void resized() override;
};

class SequenceTimelineSeeker :
	public Component,
	public ControllableContainer::ContainerAsyncListener
{
public:
	SequenceTimelineSeeker(Sequence * _sequence);
	~SequenceTimelineSeeker();

	Sequence * sequence;
	SeekHandle handle;

	//interaction
	float viewStartAtMouseDown;
	float viewEndAtMouseDown;
	float timeAnchor;

	void paint(Graphics &g) override;
	void resized() override;

	void mouseDown(const MouseEvent &e) override;
	void mouseDrag(const MouseEvent &e) override;

	int getXForTime(float time);
	float getTimeForX(int tx);

	void newMessage(const ContainerAsyncEvent &e) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceTimelineSeeker)

};


#endif  // SEQUENCETIMELINESEEKER_H_INCLUDED
