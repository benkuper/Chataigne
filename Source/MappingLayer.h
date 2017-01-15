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
#include "Mapping.h"

class MappingLayer :
	public SequenceLayer
{
public:
	MappingLayer(Sequence * _sequence);
	~MappingLayer();

	FloatParameter * curveValue; //later : float / point2d / point3d / color (4d) for multi curve layer
	FloatParameter * mappedValue; //later : polymorph out from mapping, depending on filters

	Automation automation; //later: automation Manager to allow for multi curve layer
	Mapping mapping;


	var getJSONData() override;
	void loadJSONDataInternal(var data) override;


	static MappingLayer * create(Sequence * sequence) { return new MappingLayer(sequence); }
	virtual String getTypeString() const override { return "Mapping"; }

	virtual SequenceLayerPanel * getPanel() override;
	virtual SequenceLayerTimeline * getTimelineUI() override;

	virtual void sequenceTotalTimeChanged(Sequence *) override;
	virtual void sequenceCurrentTimeChanged(Sequence *, float /*prevTime */, bool /*evaluateSkippedData */) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MappingLayer)
};




#endif  // MAPPINGLAYER_H_INCLUDED
