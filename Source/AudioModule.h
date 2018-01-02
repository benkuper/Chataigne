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
#include "pitch/PitchMPM.h"

#define AUDIO_INPUT_GRAPH_ID 1
#define AUDIO_OUTPUT_GRAPH_ID 2

class AudioModule :
	public Module,
	public AudioIODeviceCallback,
	public ChangeListener
{
public:
	AudioModule(const String &name = "Sound Card");
	~AudioModule();

	AudioDeviceManager am;
	AudioProcessorPlayer player;
	AudioProcessorGraph graph;
	
	double currentSampleRate;
	int currentBufferSize;

	BoolParameter * keepLastDetectedValues;
	
	int uidIncrement;

	const int analysisSamples = 1024;
	int curBufferIndex;
	AudioBuffer<float> buffer;

	//Parameters
	FloatParameter * inputGain;
	FloatParameter * activityThreshold;

	ControllableContainer monitorParams;
	FloatParameter * monitorVolume;
	Array<BoolParameter *> monitorOutChannels;
	Array<int> selectedMonitorOutChannels;
	int numActiveMonitorOutputs;


	//Values
	FloatParameter * volume;
	FloatParameter * frequency;
	IntParameter * pitch;

	EnumParameter * note;
	IntParameter * octave;

	ScopedPointer<PitchMPM> pitchDetector;

	void updateSelectedMonitorChannels();

	void onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c) override;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	// Inherited via AudioIODeviceCallback
	virtual void audioDeviceIOCallback(const float ** inputChannelData, int numInputChannels, float ** outputChannelData, int numOutputChannels, int numSamples) override;
	virtual void audioDeviceAboutToStart(AudioIODevice * device) override;
	virtual void audioDeviceStopped() override;

	// Inherited via ChangeListener
	virtual void changeListenerCallback(ChangeBroadcaster * source) override;



	static AudioModule * create() { return new AudioModule(); }
	virtual String getDefaultTypeString() const override { return "Sound Card"; }

	class AudioModuleListener
	{
	public:
		virtual ~AudioModuleListener() {}
		virtual void monitorSetupChanged() {}
	};

	ListenerList<AudioModuleListener> audioModuleListeners;
	void addAudioModuleListener(AudioModuleListener* newListener) { audioModuleListeners.add(newListener); }
	void removeAudioModuleListener(AudioModuleListener* listener) { audioModuleListeners.remove(listener); }




private:
	int getNoteForFrequency(float freq);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioModule)
};

#endif  // AUDIOMODULE_H_INCLUDED
