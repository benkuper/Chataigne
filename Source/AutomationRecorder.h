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
	public EnablingControllableContainer
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
	float timeAtRecord;
	float timeOffset;

	void setCurrentInput(Parameter * input);

	void clearKeys();
	void addKey(float value);

	void startRecording(float timeOffset = 0);
	void cancelRecording();
	Array<Point<float>> stopRecordingAndGetKeys();

	bool shouldRecord();
	

	void onContainerParameterChanged(Parameter * p) override;
	void onExternalParameterChanged(Parameter * p) override;

private:
	Array<Point<float>> getSimplifiedKeys(Array<Point<float>> arr, float epsilon);
	const std::pair<int, float> findMaximumDistance(Array<Point<float>> arr);
};