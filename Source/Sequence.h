/*
  ==============================================================================

    Sequence.h
    Created: 28 Oct 2016 8:13:19pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef SEQUENCE_H_INCLUDED
#define SEQUENCE_H_INCLUDED

#include "BaseItem.h"

class SequenceLayerManager;

class Sequence :
	public BaseItem
{
public:
	Sequence();
	virtual ~Sequence();

	FloatParameter * totalTime;
	FloatParameter * currentTime;
	FloatParameter * playSpeed;
	IntParameter * fps;

	Trigger * playTrigger;
	Trigger * pauseTrigger;
	Trigger * stopTrigger;

	BoolParameter * isPlaying;

	ScopedPointer<SequenceLayerManager> layerManager;


	//UI
	const float minViewTime = 1; //in seconds
	FloatParameter * viewStartTime;
	FloatParameter * viewEndTime;


	void onContainerParameterChangedInternal(Parameter *);
	void onContainerTriggerTriggered(Trigger *) override;

	class SequenceListener
	{
	public:
		virtual ~SequenceListener() {}
		virtual void sequencePlayStateChanged(Sequence *) {}
		virtual void sequenceCurrentTimeChanged(Sequence * , bool /*evaluateSkippedData*/) {}
	};

	ListenerList<SequenceListener> sequenceListeners;
	void addSequenceListener(SequenceListener* newListener) { sequenceListeners.add(newListener); }
	void removeSequenceListener(SequenceListener* listener) { sequenceListeners.remove(listener); }


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Sequence)
};




#endif  // SEQUENCE_H_INCLUDED
