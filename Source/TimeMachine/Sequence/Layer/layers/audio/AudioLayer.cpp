/*
  ==============================================================================

	AudioLayer.cpp
	Created: 20 Nov 2016 3:08:41pm
	Author:  Ben Kuper

  ==============================================================================
*/

#include "AudioLayer.h"
#include "ui/AudioLayerPanel.h"
#include "ui/AudioLayerTimeline.h"


AudioLayer::AudioLayer(Sequence * _sequence, var params) :
	SequenceLayer(_sequence, "New Audio Layer"),
	audioModule(nullptr),
	currentProcessor(nullptr),
	channelsCC("Channels"),
	numActiveOutputs(0), 
    graphID(0) //was -1 but since 5.2.1, generated warning. Should do otherwise ?
{
	ModuleManager::getInstance()->addBaseManagerListener(this);
	
	helpID = "AudioLayer";
    
    volume = addFloatParameter("Volume","Volume multiplier for the layer",1,0,10);
    
	enveloppe = addFloatParameter("Enveloppe", "Enveloppe", 0, 0, 1);
	enveloppe->isControllableFeedbackOnly = true;

	color->setColor(AUDIO_COLOR);

	addChildControllableContainer(&channelsCC);
	addChildControllableContainer(&clipManager);

	//if already an audio module, assign it
	for (auto &i : ModuleManager::getInstance()->items)
	{
		AudioModule * a = dynamic_cast<AudioModule *>(i);
		if (a != nullptr)
		{
			setAudioModule(a);
			break;
		}
	}

	//Should move that in UI stuff ?
	if (audioModule == nullptr)
	{
		int result = AlertWindow::showYesNoCancelBox(AlertWindow::WarningIcon, "Sound Card Module is required", "This Audio layer needs a Sound Card module to be able to actually output sound. Do you want to create one now ?","Yes", "No", "Cancel");
		if (result == 1)
		{
			AudioModule * m = AudioModule::create();
			ModuleManager::getInstance()->addItem(m);
			setAudioModule(m);
		}
	}

}

AudioLayer::~AudioLayer()
{
	if (ModuleManager::getInstanceWithoutCreating() != nullptr) ModuleManager::getInstance()->removeBaseManagerListener(this);
	setAudioModule(nullptr);
}

void AudioLayer::setAudioModule(AudioModule * newModule)
{
	if (audioModule != nullptr)
	{
        audioModule->removeAudioModuleListener(this);
		audioModule->graph.removeNode(graphID);
		currentProcessor->clear();
		currentProcessor = nullptr;

		if (!isCurrentlyLoadingData)
		{
			channelsData = channelsCC.getJSONData();
			DBG("KEEP ghost " << channelsData.toString());
		}

		channelsCC.clear();
	}

	audioModule = newModule;

	if (audioModule != nullptr)
	{
        
		currentProcessor = new AudioLayerProcessor(this);
		
		graphID = AudioProcessorGraph::NodeID(audioModule->uidIncrement++);
		audioModule->graph.addNode(currentProcessor, graphID);
		
		int numChannels = audioModule->graph.getMainBusNumOutputChannels();
		AudioChannelSet channelSet = audioModule->graph.getChannelLayoutOfBus(false, 0);

		for (int i = 0; i < numChannels; i++)
		{
			String channelName = AudioChannelSet::getChannelTypeName(channelSet.getTypeOfChannel(i));
			BoolParameter * b = channelsCC.addBoolParameter("Channel Out : " + channelName, "If enabled, sends audio from this layer to this channel", false);
			b->setValue(i < 2, false);
		}
		
		if (!channelsData.isVoid())
		{
			DBG("LOAD data " << JSON::toString(channelsData));
			channelsCC.loadJSONData(channelsData);
			channelsData = var();
		}

        audioModule->addAudioModuleListener(this);

	}

	updateSelectedOutChannels();

	//DBG("AudioLayer audio Module > " << (audioModule != nullptr ? audioModule->niceName : "null"));
	audioLayerListeners.call(&AudioLayerListener::targetAudioModuleChanged, this);
}

void AudioLayer::updateCurrentClip()
{
	AudioLayerClip * target = nullptr;
	
	if (sequence->currentTime->floatValue() > 0 || sequence->isPlaying->boolValue()) // only find a clip if sequence not at 0 or is playing
	{
		if (currentClip != nullptr && currentClip->isInRange(sequence->currentTime->floatValue())) return;
		target = clipManager.getClipAtTime(sequence->currentTime->floatValue());
	}

	if (target == currentClip) return;

	if (currentClip != nullptr)
	{
		currentClip->setIsCurrent(false);
	}

	currentClip = target;

	if (currentClip != nullptr)
	{
		currentClip->transportSource.setPosition(sequence->hiResAudioTime - currentClip->time->floatValue());
		currentClip->setIsCurrent(true);
        currentClip->channelRemapAudioSource.prepareToPlay(audioModule->currentBufferSize, audioModule->currentSampleRate);
        currentClip->channelRemapAudioSource.clearAllMappings();
        for (int i =0;i<numActiveOutputs;i++) currentClip->channelRemapAudioSource.setOutputChannelMapping(i, i);
        
		if (sequence->isPlaying->boolValue())
        {
            
            currentClip->transportSource.start();
        }
	}

}

void AudioLayer::itemAdded(Module * m)
{
	AudioModule * am = dynamic_cast<AudioModule *>(m);
	if (audioModule == nullptr &&  am != nullptr) setAudioModule(am);
}

void AudioLayer::itemRemoved(Module * m)
{
	if (audioModule == m)
	{
		setAudioModule(nullptr);
	}
}

void AudioLayer::updateSelectedOutChannels()
{
	selectedOutChannels.clear();

	if (audioModule == nullptr) return;

	audioModule->graph.disconnectNode(graphID);

	numActiveOutputs = 0;
	for (int i = 0; i < channelsCC.controllables.size(); i++) if (((BoolParameter *)channelsCC.controllables[i])->boolValue()) numActiveOutputs++;

	currentProcessor->setPlayConfigDetails(0, numActiveOutputs, audioModule->currentSampleRate, audioModule->currentBufferSize);
	currentProcessor->prepareToPlay(audioModule->currentSampleRate, audioModule->currentBufferSize);
    

	for (auto & c : clipManager.items)
	{
		c->channelRemapAudioSource.clearAllMappings();
		c->channelRemapAudioSource.prepareToPlay(audioModule->currentBufferSize, audioModule->currentSampleRate);
	}

	int index = 0;
	for (int i = 0; i < channelsCC.controllables.size(); i++)
	{
		if (((BoolParameter *)channelsCC.controllables[i])->boolValue())
		{
			selectedOutChannels.add(i);
			audioModule->graph.addConnection({{AudioProcessorGraph::NodeID(graphID), index }, {AudioProcessorGraph::NodeID(AUDIO_OUTPUT_GRAPH_ID), i } });
			for (auto & c : clipManager.items) c->channelRemapAudioSource.setOutputChannelMapping(index, index);
			index++;
		}
	}

	numActiveOutputs = selectedOutChannels.size();

}

void AudioLayer::audioSetupChanged()
{
    setAudioModule(audioModule); //force recreate out channels
}

void AudioLayer::onContainerParameterChangedInternal(Parameter * p)
{
	SequenceLayer::onContainerParameterChangedInternal(p);

	//DBG("audio layer container parameter changed internal " << p->niceName);


}

void AudioLayer::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c)
{
	SequenceLayer::onControllableFeedbackUpdateInternal(cc, c);

	if (cc == &channelsCC)
	{
		updateSelectedOutChannels();
	}
}

var AudioLayer::getJSONData()
{
	var data = SequenceLayer::getJSONData();
	data.getDynamicObject()->setProperty("clipManager", clipManager.getJSONData());
	if (audioModule != nullptr)
	{
		data.getDynamicObject()->setProperty("audioModule", audioModule->shortName);
		data.getDynamicObject()->setProperty("channels", channelsCC.getJSONData());
	}

	return data;
}

void AudioLayer::loadJSONDataInternal(var data)
{
	channelsData = data.getProperty("channels", var());


	SequenceLayer::loadJSONDataInternal(data);
	clipManager.loadJSONData(data.getProperty("clipManager", var()));
	if (data.getDynamicObject()->hasProperty("audioModule")) setAudioModule(dynamic_cast<AudioModule *>(ModuleManager::getInstance()->getItemWithName(data.getProperty("audioModule", ""))));
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
	if (currentProcessor != nullptr) enveloppe->setValue(currentProcessor->currentEnveloppe);
	else enveloppe->setValue(0);
	updateCurrentClip();
	if (currentClip != nullptr && sequence->isSeeking)
	{
        if(currentClip->transportSource.isPlaying()) currentClip->transportSource.stop();
        currentClip->transportSource.setPosition(sequence->hiResAudioTime - currentClip->time->floatValue());
        if(sequence->isPlaying->boolValue()) currentClip->transportSource.start();
	}
}

void AudioLayer::sequencePlayStateChanged(Sequence *)
{
	//
	if (!sequence->isPlaying->boolValue())
	{
		enveloppe->setValue(0);
		if (currentClip != nullptr) currentClip->transportSource.stop();
	}
	else
	{

		if (currentClip != nullptr)
		{
			currentClip->transportSource.setPosition(sequence->hiResAudioTime - currentClip->time->floatValue()); 
			currentClip->transportSource.start();
		}
	}
}


//Audio Processor

AudioLayerProcessor::AudioLayerProcessor(AudioLayer * _layer) :
	layer(_layer),
	rmsCount(0),
	tempRMS(0),
	currentEnveloppe(0)
{
}

AudioLayerProcessor::~AudioLayerProcessor()
{
	layer = nullptr;
}

void AudioLayerProcessor::clear()
{
	layer = nullptr;
}

const String AudioLayerProcessor::getName() const
{
	return String();
}

void AudioLayerProcessor::prepareToPlay(double sampleRate, int maximumExpectedSamplesPerBlock)
{
}

void AudioLayerProcessor::releaseResources()
{

}

void AudioLayerProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer & midiMessages)
{
	
	bool noProcess = false;

	WeakReference<AudioLayerClip> currentClip;

	if (layer != nullptr)
	{
		if (!layer->enabled->boolValue()) noProcess = true;;
		currentClip = layer->currentClip;
		if (currentClip.wasObjectDeleted() || currentClip.get() == nullptr) noProcess = true;
		else if (currentClip->filePath->stringValue().isEmpty()) noProcess = true;
	} else
	{
		noProcess = true;
	}

	if (buffer.getNumChannels() == 0)
	{
		noProcess = true;
	}

	if (noProcess)
	{
		currentEnveloppe = 0;
		rmsCount = 0;
		tempRMS = 0;
        buffer.clear();
        return;
	}
	
	float volumeFactor = currentClip->volume->floatValue() * layer->volume->floatValue() * layer->audioModule->outVolume->floatValue();
	layer->currentClip->transportSource.setGain(volumeFactor);

	

	AudioSourceChannelInfo bufferToFill;
	bufferToFill.buffer = &buffer;
	bufferToFill.startSample = 0;
	bufferToFill.numSamples = buffer.getNumSamples();
    
	layer->currentClip->channelRemapAudioSource.getNextAudioBlock(bufferToFill);

	float rms = 0;
	for (int i = 0; i < buffer.getNumChannels(); i++)
	{
		rms = jmax(rms, buffer.getRMSLevel(i, bufferToFill.startSample, bufferToFill.numSamples));
	}

	layer->enveloppe->setValue(rms);

	/*1
	double position = layer->sequence->hiResAudioTime - layer->currentClip->time->floatValue();

	int curSamplePos = currentClip->clipSamplePos;
	int targetSamplePos = position * currentClip->sampleRate;

	//smoothing vynil-like when not playing
	if (!layer->sequence->isPlaying->boolValue() && currentClip->scratch->boolValue()) targetSamplePos = curSamplePos + (targetSamplePos - curSamplePos) *.1f;


	int numDiffSamples = abs(targetSamplePos - curSamplePos);
	if (numDiffSamples == 0) return;

	int numBufferSamples = buffer.getNumSamples();

   

	int bufferChannels = buffer.getNumChannels();

	int numChannels = std::min(bufferChannels, layer->numActiveOutputs);
	int numBufferChannels = clipBuffer->getNumChannels();
	int maxChannels = std::max(numChannels, numBufferChannels);

	//DBG("Num channels " << bufferChannels << "/" << layer->numActiveOutputs);

	//int samplePosition = layer->currentClip->clipSamplePos;// *layer->currentClip->sampleRate;
	//int maxSamples = jmin<int>(buffer.getNumSamples(), clipBuffer->getNumSamples() - samplePosition)

	if (numChannels > 0)
	{
		for (int i = 0; i < maxChannels; i++)
		{
			int targetBufferChannel = i%numChannels;
			int targetChannel = i%numBufferChannels;

			int prevSampleInClip = 0;

			for (int j = 0; j < numBufferSamples; j++)
			{
				float val = 0;
				int sampleInClip = jlimit<int>(0, clipBuffer->getNumSamples() - 1, curSamplePos + (targetSamplePos - curSamplePos) * j*1.f / numBufferSamples);

				if (j > 0 && sampleInClip != prevSampleInClip)
				{
					int step = sampleInClip > prevSampleInClip ? 1 : -1;
					for (int k = prevSampleInClip; k != sampleInClip; k += step)
					{
						val += clipBuffer->getSample(targetChannel, k);
					}
					val /= abs(sampleInClip - prevSampleInClip);
				} else
				{
					val = clipBuffer->getSample(targetChannel, sampleInClip);
				}

				//DBG("Add sample in channel " << i);
				buffer.addSample(targetBufferChannel, j, val * volumeFactor);

				prevSampleInClip = sampleInClip;

			}
			//buffer.copyFrom(targetChannel, 0, *clipBuffer, i, samplePosition, maxSamples);
		}
	}
	

	currentClip->clipSamplePos = targetSamplePos;

	tempRMS += buffer.getRMSLevel(0, 0, buffer.getNumSamples());
	if (rmsCount*buffer.getNumSamples() > minEnveloppeSamples)
	{
		currentEnveloppe = tempRMS / rmsCount;
		tempRMS = 0;
		rmsCount = 0;
	} else
	{
		rmsCount++;
	}
	*/
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
