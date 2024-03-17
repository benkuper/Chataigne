/*
  ==============================================================================

	AudioLayer.cpp
	Created: 20 Nov 2016 3:08:41pm
	Author:  Ben Kuper

  ==============================================================================
*/

#include "TimeMachine/TimeMachineIncludes.h"

ChataigneAudioLayer::ChataigneAudioLayer(ChataigneSequence* _sequence, var params) :
	AudioLayer(_sequence, params),
	audioModule(nullptr),
	chataigneSequence(_sequence),
	timeAtStartRecord(0),
	arm(nullptr)
{
	ModuleManager::getInstance()->addBaseManagerListener(this);

	helpID = "AudioLayer";

	arm = addBoolParameter("Arm", "If checked, this will record audio and save it", false);
	autoDisarm = addBoolParameter("Auto Disarm", "If checked, this will automatically set Arm to false when the sequence stops", false);


	uiHeight->setValue(80);
}

ChataigneAudioLayer::~ChataigneAudioLayer()
{
	clearItem();
}

void ChataigneAudioLayer::clearItem()
{
	AudioLayer::clearItem();
	if (ModuleManager::getInstanceWithoutCreating() != nullptr) ModuleManager::getInstance()->removeBaseManagerListener(this);
	setAudioModule(nullptr);
}

void ChataigneAudioLayer::setAudioModule(AudioModule* newModule)
{
	if (audioModule != nullptr)
	{
		audioModule->removeAudioModuleListener(this);
	}

	audioModule = newModule;

	if (audioModule != nullptr)
	{
		setAudioProcessorGraph(&audioModule->graph, AUDIO_OUTPUTMIXER_GRAPH_ID);
		audioModule->addAudioModuleListener(this);

	}
	else
	{
		setAudioProcessorGraph(nullptr);
	}

	updateSelectedOutChannels();

	audioLayerListeners.call(&ChataigneAudioLayerListener::targetAudioModuleChanged, this);
}

void ChataigneAudioLayer::updateSelectedOutChannels()
{
	AudioLayer::updateSelectedOutChannels();
	updateInputConnections();
}

void ChataigneAudioLayer::updateInputConnections(bool updatePlayConfig)
{
	if (audioModule != nullptr)
	{
		for (auto& c : inputConnections) audioModule->graph.removeConnection(c);
		inputConnections.clear();


		if (arm != nullptr && arm->boolValue())
		{
			int numInputChannels = audioModule->graph.getTotalNumInputChannels();
			for (int i = 0; i < numInputChannels; i++)
			{
				AudioProcessorGraph::Connection c = { {AUDIO_INPUTMIXER_GRAPH_ID, i}, { graphID, i } };
				inputConnections.add(c);
				audioModule->graph.addConnection(c);
			}
		}

		numActiveInputs = inputConnections.size();
	}
	else
	{
		numActiveInputs = 0;
	}

	if (updatePlayConfig) updatePlayConfigDetails();
}

AudioLayerProcessor* ChataigneAudioLayer::createAudioLayerProcessor()
{
	return new ChataigneAudioLayerProcessor(this);
}

void ChataigneAudioLayer::itemAdded(Module* m)
{
	AudioModule* am = dynamic_cast<AudioModule*>(m);
	if (audioModule == nullptr && am != nullptr) setAudioModule(am);
}

void ChataigneAudioLayer::itemsAdded(Array<Module*> modules)
{
	if (audioModule != nullptr) return;
	for (auto& m : modules)
	{
		AudioModule* am = dynamic_cast<AudioModule*>(m);
		if (am != nullptr)
		{
			setAudioModule(am);
			break;
		}
	}
}

void ChataigneAudioLayer::itemRemoved(Module* m)
{
	if (audioModule == m)
	{
		setAudioModule(nullptr);
	}
}

void ChataigneAudioLayer::itemsRemoved(Array<Module*> modules)
{
	for (auto& m : modules)
	{
		if (audioModule == m)
		{
			setAudioModule(nullptr);
			break;
		}
	}
}

float ChataigneAudioLayer::getVolumeFactor()
{
	if (audioModule == nullptr) return 0.0f;
	return AudioLayer::getVolumeFactor() * audioModule->outVolume->floatValue();
}

void ChataigneAudioLayer::exportRMS(bool toNewMappingLayer, bool toClipboard, bool dataOnly)
{
	double frameLength = 1.0 / sequence->fps->intValue();
	int numFrames = sequence->totalTime->floatValue() / frameLength;

	Array<Point<float>> values;

	AudioSampleBuffer buffer;
	AudioLayerClip* curClip = nullptr;


	for (int i = 0; i < numFrames; ++i)
	{
		float t = i * frameLength;
		float value = 0;
		AudioLayerClip* clip = (AudioLayerClip*)clipManager.getBlockAtTime(t);

		if (clip != curClip)
		{
			curClip = clip;
			if (curClip != nullptr && curClip->readerSource != nullptr)
			{
				buffer.setSize(2, (int)curClip->readerSource->getAudioFormatReader()->lengthInSamples, false, true);
				curClip->readerSource->getAudioFormatReader()->read(&buffer, 0, buffer.getNumSamples(), 0, true, true);
			}
		}


		if (curClip != nullptr)
		{
			int rmsSampleCount = frameLength * buffer.getNumSamples() / curClip->coreLength->floatValue() * 10;
			int startSample = (t - clip->time->floatValue()) * buffer.getNumSamples() / curClip->coreLength->floatValue();

			if (rmsSampleCount > 0) value = buffer.getRMSLevel(0, startSample, jmin(rmsSampleCount, buffer.getNumSamples() - startSample));
		}

		values.add({ t, value });
	}

	String s = "";
	for (int i = 0; i < values.size(); ++i)
	{
		if (i > 0) s += "\n";
		if (!dataOnly) s += String(i) + "\t" + String(values[i].x, 3) + "\t";
		s += String(values[i].y, 3);
	}


	if (toClipboard)
	{
		SystemClipboard::copyTextToClipboard(s);
		NLOG(niceName, values.size() << " keys copied to clipboard");
	}

	if (toNewMappingLayer)
	{
		chataigneSequence->addNewMappingLayerFromValues(values);
	}
}

void ChataigneAudioLayer::onContainerParameterChanged(Parameter* p)
{
	AudioLayer::onContainerParameterChanged(p);

	if (p == arm) updateInputConnections();
}

void ChataigneAudioLayer::sequenceCurrentTimeChanged(Sequence* s, float prevTime, bool evaluateSkippedData)
{
	AudioLayer::sequenceCurrentTimeChanged(s, prevTime, evaluateSkippedData);
}

void ChataigneAudioLayer::sequencePlayStateChanged(Sequence* s)
{
	AudioLayer::sequencePlayStateChanged(s);
	if (sequence->isPlaying->boolValue())
	{
		if (arm->boolValue() && currentProcessor != nullptr)
		{
			timeAtStartRecord = sequence->currentTime->floatValue();
			((ChataigneAudioLayerProcessor*)currentProcessor)->startRecording();
		}
	}
	else
	{
		if (ChataigneAudioLayerProcessor* cProc = (ChataigneAudioLayerProcessor*)currentProcessor)
		{
			if (cProc->isRecording())
			{
				cProc->stopRecording();
				AudioLayerClip* clip = (AudioLayerClip*)clipManager.addBlockAt(timeAtStartRecord);
				clip->filePath->setValue(cProc->recordingFile.getFullPathName());
			}
		}

		if (autoDisarm->boolValue()) arm->setValue(false);
	}
}

void ChataigneAudioLayer::audioSetupChanged()
{
	updateSelectedOutChannels();
}

var ChataigneAudioLayer::getJSONData()
{
	var data = AudioLayer::getJSONData();
	if (audioModule != nullptr) data.getDynamicObject()->setProperty("audioModule", audioModule->shortName);
	return data;
}

void ChataigneAudioLayer::loadJSONDataInternal(var data)
{
	channelsData = data.getProperty("channels", var());
	AudioLayer::loadJSONDataInternal(data);
	if (data.getDynamicObject()->hasProperty("audioModule")) setAudioModule(dynamic_cast<AudioModule*>(ModuleManager::getInstance()->getItemWithName(data.getProperty("audioModule", ""))));
}

SequenceLayerPanel* ChataigneAudioLayer::getPanel()
{
	return new ChataigneAudioLayerPanel(this);
}

SequenceLayerTimeline* ChataigneAudioLayer::getTimelineUI()
{
	return new ChataigneAudioLayerTimeline(this);
}

//==============================================================================

ChataigneAudioLayerProcessor::ChataigneAudioLayerProcessor(ChataigneAudioLayer* layer) :
	AudioLayerProcessor(layer),
	cal(layer),
	numInputChannels(0)
{
}

ChataigneAudioLayerProcessor::~ChataigneAudioLayerProcessor()
{
}


void ChataigneAudioLayerProcessor::startRecording()
{
	if (cal->currentGraph == nullptr) return;

	stopRecording();

	backgroundThread.startThread();

	int sampleRate = cal->currentGraph->getSampleRate();
	if (sampleRate > 0)
	{
		// Create an OutputStream to write to our destination file...
		File f = Engine::mainEngine->getFile();
		f = (f.exists() ? f.getParentDirectory() : File::getSpecialLocation(File::userDocumentsDirectory).getChildFile("Chataigne")).getChildFile("audio");
		if (!f.exists()) f.createDirectory();

		recordingFile = f.getNonexistentChildFile("recorded", ".wav", false);

		if (auto fileStream = std::unique_ptr<FileOutputStream>(recordingFile.createOutputStream()))
		{
			// Now create a WAV writer object that writes to our output stream...
			WavAudioFormat wavFormat;

			if (auto writer = wavFormat.createWriterFor(fileStream.get(), sampleRate, 1, 16, {}, 0))
			{
				fileStream.release(); // (passes responsibility for deleting the stream to the writer object that is now using it)

				// Now we'll create one of these helper objects which will act as a FIFO buffer, and will
				// write the data to disk on our background thread.
				threadedWriter.reset(new AudioFormatWriter::ThreadedWriter(writer, backgroundThread, 32768));

				recorderListeners.call(&RecorderListener::recordingStarted, numInputChannels, getSampleRate());

				// And now, swap over our active writer pointer so that the audio callback will start using it..
				const ScopedLock sl(writerLock);
				activeWriter = threadedWriter.get();
			}
		}
	}
}

void ChataigneAudioLayerProcessor::stopRecording()
{

	// First, clear this pointer to stop the audio callback from using our writer object..
	{
		const ScopedLock sl(writerLock);
		activeWriter = nullptr;
	}

	// Now we can delete the writer object. It's done in this order because the deletion could
	// take a little time while remaining data gets flushed to disk, so it's best to avoid blocking
	// the audio callback while this happens.
	threadedWriter.reset();

	backgroundThread.stopThread(500);

	recorderListeners.call(&RecorderListener::recordingStopped);
}

bool ChataigneAudioLayerProcessor::isRecording() const
{
	return activeWriter.load() != nullptr;
}

void ChataigneAudioLayerProcessor::prepareToPlay(double sampleRate, int maximumExpectedSamplesPerBlock)
{
	AudioLayerProcessor::prepareToPlay(sampleRate, maximumExpectedSamplesPerBlock);
}

void ChataigneAudioLayerProcessor::releaseResources()
{
	AudioLayerProcessor::releaseResources();
}

void ChataigneAudioLayerProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	//Do before buffer is potentially cleared
	numInputChannels = buffer.getNumChannels();
	if (isRecording() && numInputChannels > 0)
	{
		{
			const GenericScopedLock sl(writerLock);
			activeWriter.load()->write(buffer.getArrayOfReadPointers(), buffer.getNumSamples());
		}

		recorderListeners.call(&RecorderListener::recordingUpdated, buffer, buffer.getNumSamples());
	}

	AudioLayerProcessor::processBlock(buffer, midiMessages);
}
