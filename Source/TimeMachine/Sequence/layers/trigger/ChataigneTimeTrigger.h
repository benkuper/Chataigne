/*
  ==============================================================================

    ChataigneTimeTrigger.h
    Created: 20 Nov 2016 3:18:20pm
    Author:  Ben Kuper

  ==============================================================================
*/

#pragma once

#include "Common/Processor/Action/Action.h"

class ChataigneTimeTrigger :
	public TimeTrigger
{
public:
	ChataigneTimeTrigger(StringRef name = "Trigger");
	virtual ~ChataigneTimeTrigger();

	ConsequenceManager csm;

	virtual void onContainerParameterChangedInternal(Parameter* p) override;

	virtual void triggerInternal() override;

	virtual var getJSONData() override;
	virtual void loadJSONDataInternal(var data) override;
};