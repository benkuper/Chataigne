/*
  ==============================================================================

    TriggerLayer.h
    Created: 17 Nov 2016 7:59:54pm
    Author:  Ben Kuper

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class ActionLayer;

class TimeActionManager :
	public TimeTriggerManager
{
public:
	TimeActionManager(ActionLayer * layer, Sequence* sequence);
	~TimeActionManager();

	TimeTrigger * createItem() override;
};


class ActionLayer :
	public TriggerLayer
{
public :
	ActionLayer(Sequence * _sequence, var params = var());
	~ActionLayer();

	static ActionLayer* create(Sequence* sequence, var params) { return new ActionLayer(sequence, params); }
	virtual String getTypeString() const override { return "Action"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ActionLayer)
};