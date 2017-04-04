/*
  ==============================================================================

	AudioLayer.cpp
	Created: 20 Nov 2016 3:08:41pm
	Author:  Ben Kuper

  ==============================================================================
*/

#include "AudioLayer.h"
#include "AudioLayerPanel.h"
#include "AudioLayerTimeline.h"


AudioLayer::AudioLayer(Sequence * _sequence) :
	SequenceLayer(_sequence, "New Audio Layer"),
	audioModule(nullptr),
	currentClip(nullptr),
	graphID(-1)
{

	enveloppe = addFloatParameter("Enveloppe", "Enveloppe", 0, 0, 1);
	enveloppe->isControllableFeedbackOnly = true;
	addChildControllableContainer(&clipManager);

}

AudioLayer::~AudioLayer()
{
	setAudioModule(nullptr);
}

void AudioLayer::setAudioModule(AudioModule * newModule)
{
	if (audioModule != nullptr)
	{
		//audioModule->graph.removeConnection(1, 0, graphID, 0); //remove In Connection
		audioModule->graph.removeConnection(graphID, 0, 2, 0); //remove Out connection
		audioModule->graph.removeNode(graphID);
	}

	audioModule = newModule;

	if (audioModule != nullptr)
	{
		currentProcessor = new AudioLayerProcessor(this);
		currentProcessor->setPlayConfigDetails(0, 2, audioModule->currentSampleRate, audioModule->currentBufferSize);
		currentProcessor->prepareToPlay(audioModule->currentSampleRate, audioModule->currentBufferSize);
		//audioModule->player.setProcessor(currentProcessor);
		graphID = audioModule->uidIncrement++;
		audioModule->graph.addNode(currentProcessor, graphID);
		//audioModule->graph.addConnection(1, 0, graphID, 0);
		audioModule->graph.addConnection(graphID, 0, 2, 0);

	}
}

void AudioLayer::updateCurrentClip()
{
	if (currentClip != nullptr && currentClip->isInRange(sequence->currentTime->floatValue())) return;
	AudioLayerClip * target = clipManager.getClipAtTime(sequence->currentTime->floatValue());

	if (target == currentClip) return;

	if (currentClip != nullptr)
	{
		currentClip->setIsCurrent(false);

	}

	currentClip = target;

	if (currentClip != nullptr)
	{
		currentClip->setIsCurrent(true);
		currentClip->clipSamplePos = (sequence->currentTime->floatValue() - currentClip->time->floatValue())*currentClip->sampleRate;
	}

}

void AudioLayer::itemAdded(Module * m)
{
	if (audioModule == nullptr && static_cast<AudioModule *>(m) != nullptr) setAudioModule(static_cast<AudioModule *>(m));
}

void AudioLayer::itemRemoved(Module * m)
{
	if (audioModule == m) setAudioModule(nullptr);
}

var AudioLayer::getJSONData()
{
	var data = SequenceLayer::getJSONData();
	data.getDynamicObject()->setProperty("clipManager", clipManager.getJSONData());
	if (audioModule != nullptr) data.getDynamicObject()->setProperty("audioModule", audioModule->shortName);
	return data;
}

void AudioLayer::loadJSONDataInternal(var data)
{
	SequenceLayer::loadJSONDataInternal(data);
	clipManager.loadJSONData(data.getProperty("clipManager", var()));
	if (data.getDynamicObject()->hasProperty("audioModule")) setAudioModule(static_cast<AudioModule *>(ModuleManager::getInstance()->getItemWithName(data.getProperty("audioModule", ""))));
}

SequenceLayerPanel * AudioLayer::getPanel()
{
	return new AudioLayerPanel(this);
}

SequenceLayerTimeline * AudioLayer::getTimelineUI()
{
	return new AudioLayerTimeline(this);
}

void AudioLayer::sequenceCurrentTimeChanged(Sequence *, float, bool)
{
	updateCurrentClip();
}

void AudioLayer::sequencePlayStateChanged(Sequence *)
{
	//
}


//Audio Processor

AudioLayerProcessor::AudioLayerProcessor(AudioLayer * _layer) :
	layer(_layer)
{
}

const String AudioLayerProcessor::getName() const
{
	return String();
}

void AudioLayerProcessor::prepareToPlay(double sampleRate, int maximumExpectedSamplesPerBlock)
{
	DBG("Prepare to play : " << sampleRate << " /" << maximumExpectedSamplesPerBlock);
}

void AudioLayerProcessor::releaseResources()
{

}

void AudioLayerProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer & midiMessages)
{
	buffer.clear();
	if (layer->currentClip == nullptr) return;
	if (layer->currentClip->filePath->stringValue().isEmpty()) return;
	if (!layer->sequence->isPlaying->boolValue()) return;

	AudioSampleBuffer * clipBuffer = &layer->currentClip->buffer;
	//int numClipSamples = clipBuffer->getNumSamples();

	float position = layer->sequence->currentTime->floatValue() - layer->currentClip->time->floatValue();
	int samplePosition = position *layer->currentClip->sampleRate;

	//int samplePosition = layer->currentClip->clipSamplePos;// *layer->currentClip->sampleRate;
	
	int maxSamples = jmin<int>(buffer.getNumSamples(), clipBuffer->getNumSamples() - samplePosition);
	for (int i = 0; i < buffer.getNumChannels(); i++)
	{
		buffer.copyFrom(i, 0, *clipBuffer, i, samplePosition, maxSamples);
	}

	//layer->currentClip->clipSamplePos += maxSamples;

	layer->enveloppe->setValue(buffer.getRMSLevel(0, 0, buffer.getNumSamples()));
}

double AudioLayerProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

bool AudioLayerProcessor::acceptsMidi() const
{
	return false;
}

bool AudioLayerProcessor::producesMidi() const
{
	return false;
}

AudioProcessorEditor * AudioLayerProcessor::createEditor()
{
	return nullptr;
}

bool AudioLayerProcessor::hasEditor() const
{
	return false;
}

int AudioLayerProcessor::getNumPrograms()
{
	return 0;
}

int AudioLayerProcessor::getCurrentProgram()
{
	return 0;
}

void AudioLayerProcessor::setCurrentProgram(int index)
{
}

const String AudioLayerProcessor::getProgramName(int index)
{
	return String();
}

void AudioLayerProcessor::changeProgramName(int index, const String & newName)
{
}

void AudioLayerProcessor::getStateInformation(juce::MemoryBlock & destData)
{
}

void AudioLayerProcessor::setStateInformation(const void * data, int sizeInBytes)
{
}
