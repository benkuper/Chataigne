/*
  ==============================================================================

	AutomationRecorder.h
	Created: 12 Jan 2018 9:55:22pm
	Author:  Ben

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"


class AutomationRecorder :
	public EnablingControllableContainer,
	public Inspectable::InspectableListener
{
public:
	AutomationRecorder();
	~AutomationRecorder();

	TargetParameter * input;
	FloatParameter * simplificationFactor;
	BoolParameter * arm;

	WeakReference<Parameter> currentInput;

	BoolParameter * isRecording;

	Array<Point<float>> keys;

	void setCurrentInput(Parameter * input);

	void clearKeys();
	void addKeyAt(float time);

	void startRecording();
	void cancelRecording();
	Array<Point<float>> stopRecordingAndGetKeys();

	bool shouldRecord();

	void onContainerParameterChanged(Parameter * p) override;
	
	void inspectableDestroyed(Inspectable * i) override;

private:
	Array<Point<float>> getSimplifiedKeys(Array<Point<float>> arr, float epsilon);
	const std::pair<int, float> findMaximumDistance(Array<Point<float>> arr);
};