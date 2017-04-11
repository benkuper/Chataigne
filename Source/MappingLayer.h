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
#include "TimeColorManager.h"

class MappingLayer :
	public SequenceLayer
{
public:
	enum Mode { MODE_1D, MODE_2D, MODE_3D, MODE_COLOR };

	MappingLayer(Sequence * _sequence);
	~MappingLayer();

	EnumParameter * mode;

	Parameter * curveValue; //later : float / point2d / point3d / color (4d) for multi curve layer
	//Parameter * mappedValue; //later : polymorph out from mapping, depending on filters

	OwnedArray<Automation> automations; //later: automation Manager to allow for multi curve layer
	ScopedPointer<TimeColorManager> timeColorManager;

	Mapping mapping;

	Mode getMappingMode();

	void setupMappingForCurrentMode();

	void updateCurvesValues();

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	static MappingLayer * create(Sequence * sequence) { return new MappingLayer(sequence); }
	virtual String getTypeString() const override { return "Automation"; }

	virtual SequenceLayerPanel * getPanel() override;
	virtual SequenceLayerTimeline * getTimelineUI() override;

	virtual void onContainerParameterChangedInternal(Parameter * p) override;

	virtual void sequenceTotalTimeChanged(Sequence *) override;
	virtual void sequenceCurrentTimeChanged(Sequence *, float /*prevTime */, bool /*evaluateSkippedData */) override;

	InspectableEditor * getEditor(bool isRoot) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MappingLayer)
};




#endif  // MAPPINGLAYER_H_INCLUDED
