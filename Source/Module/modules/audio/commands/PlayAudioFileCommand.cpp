/*
  ==============================================================================

	PlayAudioFileCommand.cpp
	Created: 2 Apr 2019 7:26:18pm
	Author:  bkupe

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

PlayAudioFileCommand::PlayAudioFileCommand(AudioModule* _module, CommandContext context, var params, Multiplex* multiplex) :
	BaseCommand(_module, context, params, multiplex),
	audioModule(_module),
	channelRemapAudioSource(&transportSource, false),
	fileSampleRate(44100),
	channelsCC("Selected channels"),
	numFileChannels(2),
	numActiveOutputs(2)
{
	saveAndLoadRecursiveData = true;
	audioFile = addFileParameter("Audio file", "The Audio file to play");
	audioFile->fileTypeFilter = "*.wav;*.mp3;*.aiff";

	volume = addFloatParameter("Volume", "A multiplier to apply to the volume of the original file", 1, 0, 5);

	addChildControllableContainer(&channelsCC);

	std::unique_ptr<PlayAudioFileCommandProcessor> proc(new PlayAudioFileCommandProcessor(this));
	currentProcessor = proc.get();

	graphID = AudioProcessorGraph::NodeID(audioModule->uidIncrement++);

	audioModule->graph.addNode(std::move(proc), graphID);

	int numChannels = audioModule->graph.getMainBusNumOutputChannels();
	AudioChannelSet channelSet = audioModule->graph.getChannelLayoutOfBus(false, 0);

	for (int i = 0; i < numChannels; ++i)
	{
		String channelName = AudioChannelSet::getChannelTypeName(channelSet.getTypeOfChannel(i));
		BoolParameter* b = channelsCC.addBoolParameter("Channel Out : " + channelName, "If enabled, sends audio from this layer to this channel", false);
		b->setValue(i < 2, false);
	}

	formatManager.registerBasicFormats();
	audioModule->addAudioModuleListener(this);
	updateSelectedOutChannels();
}

PlayAudioFileCommand::~PlayAudioFileCommand()
{
	if (audioModule != nullptr && !audioModule->isClearing)
	{
		audioModule->removeAudioModuleListener(this);
		audioModule->graph.removeNode(graphID);
	}

	currentProcessor->clear();
	currentProcessor = nullptr;
}

void PlayAudioFileCommand::updateSelectedOutChannels()
{
	selectedOutChannels.clear();

	if (audioModule == nullptr) return;

	audioModule->graph.disconnectNode(graphID);

	numActiveOutputs = 0;
	for (int i = 0; i < channelsCC.controllables.size(); ++i) if (((BoolParameter*)channelsCC.controllables[i])->boolValue()) numActiveOutputs++;

	currentProcessor->setPlayConfigDetails(0, numActiveOutputs, audioModule->currentSampleRate, audioModule->currentBufferSize);
	currentProcessor->prepareToPlay(audioModule->currentSampleRate, audioModule->currentBufferSize);

	channelRemapAudioSource.clearAllMappings();


	int index = 0;
	for (int i = 0; i < channelsCC.controllables.size(); ++i)
	{
		if (((BoolParameter*)channelsCC.controllables[i])->boolValue())
		{
			selectedOutChannels.add(i);
			audioModule->graph.addConnection({ {AudioProcessorGraph::NodeID(graphID), index }, {AudioProcessorGraph::NodeID(AUDIO_OUTPUT_GRAPH_ID), i } });
			channelRemapAudioSource.setOutputChannelMapping(index, index);
			index++;
		}
	}

	channelRemapAudioSource.setNumberOfChannelsToProduce(selectedOutChannels.size());
	numActiveOutputs = selectedOutChannels.size();
}



void PlayAudioFileCommand::setAudioFile(File f)
{
	transportSource.setSource(nullptr);
	readerSource.reset(nullptr);
	if (!f.exists()) return;

	AudioFormatReader* reader = formatManager.createReaderFor(f);

	if (reader != nullptr)
	{
		std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
		transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate, reader->numChannels);
		readerSource.reset(newSource.release());
		fileSampleRate = reader->sampleRate;
		numFileChannels = reader->numChannels;
	}

	updateSelectedOutChannels();
}

void PlayAudioFileCommand::onContainerParameterChanged(Parameter* p)
{
	BaseCommand::onContainerParameterChanged(p);

	if (p == audioFile)
	{
		setAudioFile(audioFile->getFile());
	}
}

void PlayAudioFileCommand::onControllableFeedbackUpdate(ControllableContainer* cc, Controllable* c)
{
	BaseCommand::onControllableFeedbackUpdate(cc, c);

	if (cc == &channelsCC)
	{
		updateSelectedOutChannels();
	}
}

void PlayAudioFileCommand::triggerInternal(int multiplexIndex)
{
	if (readerSource.get() == nullptr) return;
	transportSource.setPosition(0);
	transportSource.start();
	audioModule->outActivityTrigger->trigger();
}

// PROCESSOR

PlayAudioFileCommandProcessor::PlayAudioFileCommandProcessor(PlayAudioFileCommand* _command) :
	command(_command),
	rmsCount(0),
	tempRMS(0),
	currentEnveloppe(0)
{
}

PlayAudioFileCommandProcessor::~PlayAudioFileCommandProcessor()
{
	command = nullptr;
}

void PlayAudioFileCommandProcessor::clear()
{
	command = nullptr;
}

const String PlayAudioFileCommandProcessor::getName() const
{
	return String();
}

void PlayAudioFileCommandProcessor::prepareToPlay(double sampleRate, int maximumExpectedSamplesPerBlock)
{
	if (command != nullptr && command->readerSource.get() != nullptr) command->channelRemapAudioSource.prepareToPlay(maximumExpectedSamplesPerBlock, sampleRate);
}

void PlayAudioFileCommandProcessor::releaseResources()
{
	if (command != nullptr) command->channelRemapAudioSource.releaseResources();
}

void PlayAudioFileCommandProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	if (command == nullptr || command->readerSource.get() == nullptr || !command->transportSource.isPlaying())
	{
		buffer.clear();
		return;
	}

	AudioSourceChannelInfo bufferToFill;
	bufferToFill.buffer = &buffer;
	bufferToFill.startSample = 0;
	bufferToFill.numSamples = buffer.getNumSamples();

	command->channelRemapAudioSource.getNextAudioBlock(bufferToFill);
	bufferToFill.buffer->applyGain(command->volume->floatValue());
}

double PlayAudioFileCommandProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

bool PlayAudioFileCommandProcessor::acceptsMidi() const
{
	return false;
}

bool PlayAudioFileCommandProcessor::producesMidi() const
{
	return false;
}

AudioProcessorEditor* PlayAudioFileCommandProcessor::createEditor()
{
	return nullptr;
}

bool PlayAudioFileCommandProcessor::hasEditor() const
{
	return false;
}

int PlayAudioFileCommandProcessor::getNumPrograms()
{
	return 0;
}

int PlayAudioFileCommandProcessor::getCurrentProgram()
{
	return 0;
}

void PlayAudioFileCommandProcessor::setCurrentProgram(int index)
{
}

const String PlayAudioFileCommandProcessor::getProgramName(int index)
{
	return String();
}

void PlayAudioFileCommandProcessor::changeProgramName(int index, const String& newName)
{
}

void PlayAudioFileCommandProcessor::getStateInformation(juce::MemoryBlock& destData)
{
}

void PlayAudioFileCommandProcessor::setStateInformation(const void* data, int sizeInBytes)
{
}
