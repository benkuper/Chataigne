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
#include "TimeTrigger.h"

class TriggerLayer :
	public SequenceLayer
{
public :
	TriggerLayer();
	~TriggerLayer();

	void init() override;
	void loadJSONDataInternal(var data) override;

	static TriggerLayer * create() { return new TriggerLayer(); }
	virtual String getTypeString() const override { return "Trigger"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TriggerLayer)
};



#endif  // TRIGGERLAYER_H_INCLUDED
