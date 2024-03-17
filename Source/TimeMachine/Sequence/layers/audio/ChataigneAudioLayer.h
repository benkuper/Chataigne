/*
  ==============================================================================

    AudioLayer.h
    Created: 20 Nov 2016 3:08:41pm
    Author:  Ben Kuper

  ==============================================================================
*/

#pragma once

#include "Module/ModuleIncludes.h"

class ChataigneAudioLayerProcessor;

class ChataigneAudioLayer :
	public AudioLayer,
	public ModuleManager::ManagerListener,
    public AudioModule::AudioModuleListener
{
public:
	ChataigneAudioLayer(ChataigneSequence * sequence, var params);
	~ChataigneAudioLayer();
	
	AudioModule * audioModule;
	ChataigneSequence* chataigneSequence;

	//Recording
	BoolParameter* arm;
	BoolParameter * autoDisarm;
	Array<AudioProcessorGraph::Connection> inputConnections;
	float timeAtStartRecord;

	virtual void clearItem() override;

	void setAudioModule(AudioModule * newModule);
	void updateSelectedOutChannels() override;
	void updateInputConnections(bool updatePlayConfig = true);
	AudioLayerProcessor* createAudioLayerProcessor() override;

	void itemAdded(Module* m) override;
	void itemsAdded(Array<Module*> modules) override;
	void itemRemoved(Module* m) override;
	void itemsRemoved(Array<Module*> modules) override;

	virtual float getVolumeFactor() override;
	void exportRMS(bool toNewMappingLayer, bool toClipboard, bool dataOnly = false);


	void onContainerParameterChanged(Parameter* p) override;

	void sequenceCurrentTimeChanged(Sequence* s, float prevTime, bool evaluateSkippedData) override;
	void sequencePlayStateChanged(Sequence* s) override;
	void audioSetupChanged() override;


	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	virtual SequenceLayerPanel* getPanel() override;
	virtual SequenceLayerTimeline* getTimelineUI() override;

	static ChataigneAudioLayer * create(Sequence * sequence, var params) { return new ChataigneAudioLayer((ChataigneSequence *)sequence, params); }
	virtual String getTypeString() const override { return "Audio"; }


	ListenerList<ChataigneAudioLayerListener> audioLayerListeners;
	void addAudioLayerListener(ChataigneAudioLayerListener* newListener) { audioLayerListeners.add(newListener); }
	void removeAudioLayerListener(ChataigneAudioLayerListener* listener) { audioLayerListeners.remove(listener); }


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChataigneAudioLayer)
};

class ChataigneAudioLayerProcessor :
	public AudioLayerProcessor
{
public:
	ChataigneAudioLayerProcessor(ChataigneAudioLayer* layer);
	~ChataigneAudioLayerProcessor();

	ChataigneAudioLayer* cal;
	int numInputChannels;

	std::unique_ptr<AudioFormatWriter::ThreadedWriter> threadedWriter;
	TimeSliceThread backgroundThread{ "Audio Recorder Thread" }; // the thread that will write our audio data to disk
	CriticalSection writerLock;
	std::atomic<AudioFormatWriter::ThreadedWriter*> activeWriter{ nullptr };

	File recordingFile;

	void startRecording();
	void stopRecording();
	bool isRecording() const;

	virtual void prepareToPlay(double sampleRate, int maximumExpectedSamplesPerBlock) override;
	virtual void releaseResources() override;
	virtual void processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) override;

	class RecorderListener
	{
	public:
        virtual ~RecorderListener() {}
		virtual void recordingStarted(int numChannels, int sampleRate) {}
		virtual void recordingStopped() {}
		virtual void recordingUpdated(AudioBuffer<float>& buffer, int numSamples) {}
	};

	ListenerList<RecorderListener> recorderListeners;
	void addAudioRecorderListener(RecorderListener* newListener) { recorderListeners.add(newListener); }
	void removeAudioRecorderListener(RecorderListener* listener) { recorderListeners.remove(listener); }

};
