/*
  ==============================================================================

    MappingLayer.h
    Created: 17 Nov 2016 8:00:02pm
    Author:  Ben Kuper

  ==============================================================================
*/

#ifndef MAPPINGLAYER_H_INCLUDED
#define MAPPINGLAYER_H_INCLUDED

#include "SequenceLayer.h"
#include "Automation.h"
class MappingLayer :
	public SequenceLayer
{
public:
	MappingLayer(Sequence * _sequence);
	~MappingLayer();

	Automation automation;

	void loadJSONDataInternal(var data) override;


	static MappingLayer * create(Sequence * sequence) { return new MappingLayer(sequence); }
	virtual String getTypeString() const override { return "Mapping"; }

	virtual SequenceLayerPanel * getPanel() override;
	virtual SequenceLayerTimeline * getTimelineUI() override;

	virtual void sequenceTotalTimeChanged(Sequence *) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MappingLayer)
};




#endif  // MAPPINGLAYER_H_INCLUDED
