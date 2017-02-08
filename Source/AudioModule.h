/*
  ==============================================================================

    AudioModule.h
    Created: 6 Feb 2017 8:46:11pm
    Author:  Ben

  ==============================================================================
*/

#ifndef AUDIOMODULE_H_INCLUDED
#define AUDIOMODULE_H_INCLUDED

#include "Module.h"
#include "pitch\PitchMPM.h"

class AudioModule :
	public Module,
	public AudioIODeviceCallback,
	public ChangeListener
{
public:
	AudioModule(const String &name = "Audio Device");
	~AudioModule();

	AudioDeviceManager am;

	const int analysisSamples = 1024;
	int curBufferIndex;
	AudioBuffer<float> buffer;

	FloatParameter * gain;
	FloatParameter * activityThreshold;

	FloatParameter * volume;
	FloatParameter * frequency;
	IntParameter * pitch;

	EnumParameter * note;
	IntParameter * octave;

	ScopedPointer<PitchMPM> pitchDetector;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	// Inherited via AudioIODeviceCallback
	virtual void audioDeviceIOCallback(const float ** inputChannelData, int numInputChannels, float ** outputChannelData, int numOutputChannels, int numSamples) override;
	virtual void audioDeviceAboutToStart(AudioIODevice * device) override;
	virtual void audioDeviceStopped() override;

	// Inherited via ChangeListener
	virtual void changeListenerCallback(ChangeBroadcaster * source) override;


	InspectableEditor * getEditor(bool isRoot) override;

	static AudioModule * create() { return new AudioModule(); }
	virtual String getTypeString() const override { return "Audio Device"; }


private:
	int getNoteForFrequency(float freq);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioModule)
};

#endif  // AUDIOMODULE_H_INCLUDED
