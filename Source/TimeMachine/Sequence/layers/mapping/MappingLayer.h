/*
  ==============================================================================

	MappingLayer.h
	Created: 17 Nov 2016 8:00:02pm
	Author:  Ben Kuper

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class GradientColorManager;
class Mapping;


class MappingLayer :
	public SequenceLayer
{
public:
	MappingLayer(const String& name, Sequence* _sequence, var params);
	~MappingLayer();

	BoolParameter* alwaysUpdate;
	BoolParameter* sendOnPlay;
	BoolParameter* sendOnStop;
	BoolParameter* sendOnSeek;

	Parameter* mappingInputSource;
	Parameter* mappingInput;
	std::unique_ptr<Mapping> mapping;

	void setupMappingInputParameter(Parameter* source);

	void updateMappingInputValue(bool forceOutput = false);
	virtual void updateMappingInputValueInternal();

	virtual var getValueAtPosition(float position) = 0;
	void exportBakedValues(bool dataOnly = false);

	virtual void onContainerParameterChangedInternal(Parameter* p) override;
	virtual void onContainerTriggerTriggered(Trigger* t) override;
	void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;
	void onExternalParameterRangeChanged(Parameter* p) override;

	void sequenceCurrentTimeChanged(Sequence*, float prevTime, bool evaluateSkippedData) override;
	virtual void sequenceCurrentTimeChangedInternal(Sequence*, float prevTime, bool evaluateSkippedData) {};
	void sequencePlayStateChanged(Sequence*) override;
	virtual void sequencePlayStateChangedInternal(Sequence*) {}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MappingLayer)
};