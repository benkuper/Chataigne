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
	
	MappingLayer(Sequence* _sequence, var params);
	~MappingLayer();

	enum Mode { MODE_1D, MODE_2D, MODE_3D, MODE_COLOR, MODE_MAX };
	static String modeNames[MODE_MAX];

	Mode mode;

	BoolParameter* alwaysUpdate;
	BoolParameter* sendOnPlay;
	BoolParameter* sendOnStop;
	BoolParameter* sendOnSeek;

	enum RecordSendMode { DONOTSEND, SEND_ORIGINAL, SEND_NEW };
	EnumParameter* recordSendMode;

	Parameter* curveValue; //later : float / point2d / point3d / color (4d) for multi curve layer
	//Parameter * mappedValue; //later : polymorph out from mapping, depending on filters

	std::unique_ptr<Automation> automation;
	std::unique_ptr<GradientColorManager> colorManager;

	AutomationRecorder recorder;
	std::unique_ptr<Mapping> mapping;

	void setupMappingForCurrentMode();

	void updateCurvesValues();

	void stopRecorderAndAddKeys();

	String getHelpID() override;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	virtual void exportBakedValues(bool dataOnly = false);

	virtual void selectAll(bool addToSelection = false) override;

	virtual void onContainerParameterChangedInternal(Parameter* p) override;
	virtual void onContainerTriggerTriggered(Trigger* t) override;
	void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;

	virtual void sequenceTotalTimeChanged(Sequence*) override;
	virtual void sequenceCurrentTimeChanged(Sequence*, float prevTime, bool evaluateSkippedData) override;
	virtual void sequencePlayStateChanged(Sequence*) override;
	virtual void sequenceLooped(Sequence*) override;

	virtual SequenceLayerPanel* getPanel() override;
	virtual SequenceLayerTimeline* getTimelineUI() override;


	static MappingLayer* create(Sequence* sequence, var params) { return new MappingLayer(sequence, params); }
	virtual String getTypeString() const override {
		return modeNames[mode] ; 
	}


	virtual bool paste() override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MappingLayer)
};