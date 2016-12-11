/*
  ==============================================================================

    TriggerLayer.h
    Created: 17 Nov 2016 7:59:54pm
    Author:  Ben Kuper

  ==============================================================================
*/

#ifndef TRIGGERLAYER_H_INCLUDED
#define TRIGGERLAYER_H_INCLUDED

#include "SequenceLayer.h"
#include "TimeTriggerManager.h"

class TriggerLayer :
	public SequenceLayer
{
public :
	TriggerLayer(Sequence * _sequence);
	~TriggerLayer();

	TimeTriggerManager ttm;

	void loadJSONDataInternal(var data) override;

	static TriggerLayer * create(Sequence * sequence) { return new TriggerLayer(sequence); }
	virtual String getTypeString() const override { return "Trigger"; }


	virtual SequenceLayerPanel * getPanel() override;
	virtual SequenceLayerTimeline * getTimelineUI() override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TriggerLayer)
};



#endif  // TRIGGERLAYER_H_INCLUDED
