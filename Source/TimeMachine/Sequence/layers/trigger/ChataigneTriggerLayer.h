/*
  ==============================================================================

    TriggerLayer.h
    Created: 17 Nov 2016 7:59:54pm
    Author:  Ben Kuper

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class ChataigneTriggerLayer;

class ChataigneTriggerManager :
	public TimeTriggerManager
{
public:
	ChataigneTriggerManager(ChataigneTriggerLayer* layer, Sequence* sequence);
	~ChataigneTriggerManager();

	TimeTrigger* createItem() override;
};


class ChataigneTriggerLayer :
	public TriggerLayer
{
public :
	ChataigneTriggerLayer(Sequence * _sequence, var params = var());
	~ChataigneTriggerLayer();

	static ChataigneTriggerLayer* create(Sequence* sequence, var params) { return new ChataigneTriggerLayer(sequence, params); }
	virtual String getTypeString() const override { return "Trigger"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChataigneTriggerLayer)
};