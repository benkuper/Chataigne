/*
  ==============================================================================

    PlayAudioFileCommand.h
    Created: 2 Apr 2019 7:26:18pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class PlayAudioFileCommandProcessor;

class PlayAudioFileCommand :
	public BaseCommand,
	public AudioModule::AudioModuleListener
{
public:
	PlayAudioFileCommand(AudioModule * _module, CommandContext context, var params, Multiplex* multiplex);
	~PlayAudioFileCommand();

	AudioModule * audioModule;

	FileParameter * audioFile;
	FloatParameter* volume;
	PlayAudioFileCommandProcessor * currentProcessor;

	std::unique_ptr<AudioFormatReaderSource> readerSource;
	AudioFormatManager formatManager;
	AudioTransportSource transportSource;
	ChannelRemappingAudioSource channelRemapAudioSource;

	int fileSampleRate;

	ControllableContainer channelsCC;
	Array<int> selectedOutChannels;
	var channelsData; //for ghosting
	int numFileChannels;
	int numActiveOutputs;

	void updateSelectedOutChannels();
	//void audioSetupChanged() override;

	AudioProcessorGraph::NodeID graphID;

	void setAudioFile(File f);

	void onContainerParameterChanged(Parameter * p) override;
	void onControllableFeedbackUpdate(ControllableContainer * cc, Controllable * c) override;

	void triggerInternal(int multiplexIndex) override;

	static BaseCommand * create(ControllableContainer * module, CommandContext context, var params, Multiplex * multiplex) { return new PlayAudioFileCommand((AudioModule *)module, context, params, multiplex); }
};



class PlayAudioFileCommandProcessor :
	public AudioProcessor
{
public:
	PlayAudioFileCommandProcessor(PlayAudioFileCommand * command);
	~PlayAudioFileCommandProcessor();

	PlayAudioFileCommand * command;

	const int minEnveloppeSamples = 1024;
	int rmsCount;
	float tempRMS;
	float currentEnveloppe;

	void clear();

	// Hérité via AudioProcessor
	virtual const String getName() const override;
	virtual void prepareToPlay(double sampleRate, int maximumExpectedSamplesPerBlock) override;
	virtual void releaseResources() override;
	virtual void processBlock(AudioBuffer<float>& buffer, MidiBuffer & midiMessages) override;
	virtual double getTailLengthSeconds() const override;
	virtual bool acceptsMidi() const override;
	virtual bool producesMidi() const override;
	virtual AudioProcessorEditor * createEditor() override;
	virtual bool hasEditor() const override;
	virtual int getNumPrograms() override;
	virtual int getCurrentProgram() override;
	virtual void setCurrentProgram(int index) override;
	virtual const String getProgramName(int index) override;
	virtual void changeProgramName(int index, const String & newName) override;
	virtual void getStateInformation(juce::MemoryBlock & destData) override;
	virtual void setStateInformation(const void * data, int sizeInBytes) override;
};