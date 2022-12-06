/*
  ==============================================================================

	AudioModule.h
	Created: 6 Feb 2017 8:46:11pm
	Author:  Ben

  ==============================================================================
*/

#pragma once

#define AUDIO_INPUT_GRAPH_ID 1
#define AUDIO_OUTPUT_GRAPH_ID 2

class AudioModuleHardwareSettings :
	public ControllableContainer
{
public:
	AudioModuleHardwareSettings(AudioDeviceManager* am);
	~AudioModuleHardwareSettings() {}
	AudioDeviceManager* am;

	InspectableEditor* getEditorInternal(bool isRoot, Array<Inspectable*> inspectables = Array<Inspectable*>()) override;
};

class AudioModule :
	public Module,
	public AudioIODeviceCallback,
	public ChangeListener,
	public FFTAnalyzerManager::ManagerListener
{
public:
	AudioModule(const String& name = "Sound Card");
	~AudioModule();

	AudioModuleHardwareSettings hs;
	AudioDeviceManager am;
	AudioProcessorPlayer player;
	AudioProcessorGraph graph;

	double currentSampleRate;
	int currentBufferSize;

	BoolParameter* keepLastDetectedValues;

	int uidIncrement;

	const int analysisSamples = 1024;
	int curBufferIndex;
	AudioBuffer<float> buffer;

	//Parameters
	FloatParameter* inputGain;
	FloatParameter* activityThreshold;
	FloatParameter* outVolume;

	ControllableContainer inputVolumesCC;
	Array<FloatParameter*> inputVolumes;
	ControllableContainer outputVolumesCC;
	Array<FloatParameter*> outputVolumes;

	EnablingControllableContainer monitorParams;
	FloatParameter* monitorVolume;
	Array<BoolParameter*> monitorOutChannels;
	Array<int> selectedMonitorOutChannels;
	int numActiveMonitorOutputs;

	enum PitchDetectionMethod { NONE, MPM, YIN };
	EnumParameter* pitchDetectionMethod;

	//Values
	FloatParameter* detectedVolume;

	ControllableContainer noteCC;
	FloatParameter* frequency;
	IntParameter* pitch;
	EnumParameter* note;
	IntParameter* octave;

	ControllableContainer fftCC;

	EnablingControllableContainer ltcParamsCC;
	EnumParameter* ltcFPS;
	int curLTCFPS; //avoid accessing enum in audio thread
	IntParameter* ltcChannel;

	ControllableContainer ltcCC;
	BoolParameter* ltcPlaying;
	FloatParameter* ltcTime;
	int ltcFrameDropCount;

	FFTAnalyzerManager analyzerManager;

	std::unique_ptr<PitchDetector> pitchDetector;
	std::unique_ptr<LTCDecoder> ltcDecoder;

	virtual void updateAudioSetup();
	void updateSelectedMonitorChannels();

	void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;
	void onContainerParameterChangedInternal(Parameter* p) override;


	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	// Inherited via AudioIODeviceCallback
	virtual void audioDeviceIOCallbackWithContext(const float** inputChannelData,
		int numInputChannels,
		float** outputChannelData,
		int numOutputChannels,
		int numSamples,
		const AudioIODeviceCallbackContext& context) override;

	//7.0.3
	//(const float* const* inputChannelData,
	//	int numInputChannels,
	//	float* const* outputChannelData,
	//	int numOutputChannels,
	//	int numSamples,
	//	const AudioIODeviceCallbackContext& context) override;

	virtual void audioDeviceAboutToStart(AudioIODevice* device) override;
	virtual void audioDeviceStopped() override;

	// Inherited via ChangeListener
	virtual void changeListenerCallback(ChangeBroadcaster* source) override;


	void itemAdded(FFTAnalyzer* item) override;
	void itemRemoved(FFTAnalyzer* item) override;


	static AudioModule* create() { return new AudioModule(); }
	virtual String getDefaultTypeString() const override { return AudioModule::getTypeStringStatic(); }
	static String getTypeStringStatic() { return "Sound Card"; }
	
	class AudioModuleListener
	{
	public:
		virtual ~AudioModuleListener() {}
		virtual void monitorSetupChanged() {}
		virtual void audioSetupChanged() {}
	};

	ListenerList<AudioModuleListener> audioModuleListeners;
	void addAudioModuleListener(AudioModuleListener* newListener) { audioModuleListeners.add(newListener); }
	void removeAudioModuleListener(AudioModuleListener* listener) { audioModuleListeners.remove(listener); }




private:
	int getNoteForFrequency(float freq);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioModule)
};