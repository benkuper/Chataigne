/*
  ==============================================================================

    MappingLayer.h
    Created: 17 Nov 2016 8:00:02pm
    Author:  Ben Kuper

  ==============================================================================
*/

#ifndef MAPPINGLAYER_H_INCLUDED
#define MAPPINGLAYER_H_INCLUDED

#include "../../SequenceLayer.h"
#include "Common/Processor/Mapping/Mapping.h"
#include "timecolor/TimeColorManager.h"

class MappingLayer :
	public SequenceLayer
{
public:
	enum Mode { MODE_1D, MODE_2D, MODE_3D, MODE_COLOR };

	MappingLayer(Sequence * _sequence, var params);
	~MappingLayer();

	EnumParameter * mode;

	BoolParameter * alwaysUpdate;

	Parameter * curveValue; //later : float / point2d / point3d / color (4d) for multi curve layer
	//Parameter * mappedValue; //later : polymorph out from mapping, depending on filters

	OwnedArray<Automation> automations; //later: automation Manager to allow for multi curve layer
	ScopedPointer<TimeColorManager> timeColorManager;

	AutomationRecorder recorder;
	Mapping mapping;

	void setupMappingForCurrentMode();

	void updateCurvesValues();

	void stopRecorderAndAddKeys();

	String getHelpID() override;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	static MappingLayer * create(Sequence * sequence, var params) { return new MappingLayer(sequence, params); }
	virtual String getTypeString() const override { return "Mapping"; }

	virtual SequenceLayerPanel * getPanel() override;
	virtual SequenceLayerTimeline * getTimelineUI() override;

	virtual void onContainerParameterChangedInternal(Parameter * p) override;
	virtual void onContainerTriggerTriggered(Trigger * t) override;
	void onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c) override;

	virtual void sequenceTotalTimeChanged(Sequence *) override;
	virtual void sequenceCurrentTimeChanged(Sequence *, float prevTime , bool evaluateSkippedData) override;
	virtual void sequencePlayStateChanged(Sequence *) override;
	virtual void sequenceLooped(Sequence *) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MappingLayer)
};




#endif  // MAPPINGLAYER_H_INCLUDED
