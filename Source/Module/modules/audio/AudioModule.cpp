/*
  ==============================================================================

	AudioModule.cpp
	Created: 6 Feb 2017 8:46:11pm
	Author:  Ben

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

AudioModule::AudioModule(const String& name) :
	Module(name),
	hs(&am),
	uidIncrement(100),
	curBufferIndex(0),
	inputVolumesCC("Input Volumes"),
	outputVolumesCC("Output Volumes"),
	monitorParams("Monitor"),
	numActiveMonitorOutputs(0),
	noteCC("Pitch Detection"),
	fftCC("FFT Enveloppes"),
	ltcParamsCC("LTC"),
	ltcCC("LTC"),
	ltcFrameDropCount(0),
	pitchDetector(nullptr)
{
	setupIOConfiguration(true, true);

	inputGain = moduleParams.addFloatParameter("Input Gain", "Gain for the input volume", 1, 0, 10);
	activityThreshold = moduleParams.addFloatParameter("Activity Threshold", "Threshold to consider activity from the source.\nAnalysis will compute only if volume is greater than this parameter", .1f, 0, 1);
	keepLastDetectedValues = moduleParams.addBoolParameter("Keep Values", "Keep last detected values when no activity detected.", false);

	outVolume = moduleParams.addFloatParameter("Out Volume", "Global volume multiplier for all sound that is played through this module", 1, 0, 10);
	pitchDetectionMethod = moduleParams.addEnumParameter("Pitch Detection Method", "Choose how to detect the pitch.\nNone will disable the detection (for performance),\nMPM is better suited for monophonic sounds,\nYIN is better suited for high-pitched voices and music");
	pitchDetectionMethod->addOption("None", NONE)->addOption("MPM", MPM)->addOption("YIN", YIN);



	moduleParams.addChildControllableContainer(&inputVolumesCC);

	outputVolumesCC.editorIsCollapsed = true;
	moduleParams.addChildControllableContainer(&outputVolumesCC);


	monitorParams.enabled->setValue(false);
	moduleParams.addChildControllableContainer(&monitorParams);
	monitorVolume = monitorParams.addFloatParameter("Monitor Volume", "Volume multiplier for the monitor output. This will affect all the input channels and all the selected output channels", 1, 0, 10);

	moduleParams.addChildControllableContainer(&analyzerManager);
	analyzerManager.addBaseManagerListener(this);


	//LTC
	ltcParamsCC.enabled->setValue(false);
	moduleParams.addChildControllableContainer(&ltcParamsCC);
	ltcFPS = ltcParamsCC.addEnumParameter("FPS", "The framerate to use to decode LTC");
	ltcFPS->addOption("24", 24)->addOption("25", 25)->addOption("30", 30);
	ltcFPS->setDefaultValue(30);
	curLTCFPS = ltcFPS->getValueData();

	ltcChannel = ltcParamsCC.addIntParameter("LTC Channel", "Enable and select the channel you want to use to decode LTC", 1, 1, 64);
	ltcUseDate = ltcParamsCC.addBoolParameter("Use LTC Date", "Does the sending device use the date in the LTC user bits? (Almost always false)", false);

	//Values
	detectedVolume = valuesCC.addFloatParameter("Volume", "Volume of the audio input", 0, 0, 1);

	//Pitch Detection
	frequency = noteCC.addFloatParameter("Freq", "Freq", 0, 0, 2000);
	pitch = noteCC.addIntParameter("Pitch", "Pitch", 0, 0, 300);

	note = noteCC.addEnumParameter("Note", "Detected note");
	note->addOption("-", -1);
	for (int i = 0; i < 12; ++i) note->addOption(MIDIManager::getNoteName(i, false), i);

	octave = noteCC.addIntParameter("Octave", "Detected octave", 0, 0, 10);
	valuesCC.addChildControllableContainer(&noteCC);

	//FFT
	valuesCC.addChildControllableContainer(&fftCC);


	//LTC
	valuesCC.addChildControllableContainer(&ltcCC);
	ltcPlaying = ltcCC.addBoolParameter("Is LTC Playing", "Is LTC currently detected in audio input ?", false);
	ltcTime = ltcCC.addFloatParameter("LTC Time", "Decoded LTC Time from the selected channel in parameters", 0, 0);
	ltcTime->defaultUI = FloatParameter::TIME;


	addChildControllableContainer(&hs);
	controllableContainers.move(controllableContainers.indexOf(&hs), controllableContainers.indexOf(&valuesCC));

	for (auto& c : valuesCC.controllables) c->setControllableFeedbackOnly(true);

	defManager->add(CommandDefinition::createDef(this, "", "Play audio file", &PlayAudioFileCommand::create));

	ltcDecoder.reset(ltc_decoder_create(1920, 32));

	initSetup();
}

AudioModule::~AudioModule()
{
	graph.clear();

	am.removeAudioCallback(&player);
	player.setProcessor(nullptr);

	am.removeAudioCallback(this);
	am.removeChangeListener(this);
}

void AudioModule::initSetup()
{
	if (Thread::getCurrentThreadId() != MessageManager::getInstance()->getCurrentMessageThread())
	{
		MessageManager::callAsync([this]() { initSetup(); });
		return;
	}

	//AUDIO
	am.addAudioCallback(this);
	am.addChangeListener(this);
	am.initialiseWithDefaultDevices(0, 2);

	am.addAudioCallback(&player);

	graph.reset();

	AudioDeviceManager::AudioDeviceSetup setup;
	am.getAudioDeviceSetup(setup);
	currentSampleRate = setup.sampleRate;
	currentBufferSize = setup.bufferSize;

	if (setup.outputDeviceName.isEmpty()) setWarningMessage("Module is not connected to an audio output");

	graph.setPlayConfigDetails(0, 6, currentSampleRate, currentBufferSize);
	graph.prepareToPlay(currentSampleRate, currentBufferSize);

	std::unique_ptr<AudioProcessorGraph::AudioGraphIOProcessor> procIn(new AudioProcessorGraph::AudioGraphIOProcessor(AudioProcessorGraph::AudioGraphIOProcessor::audioInputNode));
	std::unique_ptr<AudioProcessorGraph::AudioGraphIOProcessor> procOut(new AudioProcessorGraph::AudioGraphIOProcessor(AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode));
	graph.addNode(std::move(procIn), AUDIO_INPUT_GRAPH_ID);
	graph.addNode(std::move(procOut), AUDIO_OUTPUT_GRAPH_ID);

	auto iProc = std::unique_ptr<MixerProcessor>(new MixerProcessor(this, true));
	inputMixer = iProc.get();

	auto oProc = std::unique_ptr<MixerProcessor>(new MixerProcessor(this, false));
	outputMixer = oProc.get();

	graph.addNode(std::move(iProc), AUDIO_INPUTMIXER_GRAPH_ID);
	graph.addNode(std::move(oProc), AUDIO_OUTPUTMIXER_GRAPH_ID);

	player.setProcessor(&graph);

}

void AudioModule::updateAudioSetup()
{
	AudioDeviceManager::AudioDeviceSetup setup;
	am.getAudioDeviceSetup(setup);
	currentSampleRate = setup.sampleRate;
	currentBufferSize = setup.bufferSize;

	am.removeAudioCallback(&player);
	am.removeAudioCallback(this);

	int numSelectedInputChannelsInSetup = setup.inputChannels.countNumberOfSetBits();
	int numSelectedOutputChannelsInSetup = setup.outputChannels.countNumberOfSetBits();

	graph.setPlayConfigDetails(numSelectedInputChannelsInSetup, numSelectedOutputChannelsInSetup, currentSampleRate, currentBufferSize);
	graph.prepareToPlay(currentSampleRate, currentBufferSize);

	graph.suspendProcessing(true);

	var mData = monitorParams.getJSONData();
	for (auto& c : monitorOutChannels) monitorParams.removeControllable(c);
	monitorOutChannels.clear();

	int numChannels = graph.getMainBusNumOutputChannels();
	AudioChannelSet channelSet = graph.getChannelLayoutOfBus(false, 0);


	for (int i = 0; i < numChannels; ++i)
	{
		String channelName = AudioChannelSet::getChannelTypeName(channelSet.getTypeOfChannel(i));
		BoolParameter* b = monitorParams.addBoolParameter("Monitor Out : " + channelName, "If enabled, sends audio from this input directly to the output", i < selectedMonitorOutChannels.size());
		monitorOutChannels.add(b);
	}

	monitorParams.loadJSONData(mData);
	updateSelectedMonitorChannels();

	var inData = inputVolumesCC.getJSONData();
	for (auto& c : inputVolumes) inputVolumesCC.removeControllable(c);
	inputVolumes.clear();

	var outData = outputVolumesCC.getJSONData();
	for (auto& c : outputVolumes) outputVolumesCC.removeControllable(c);
	outputVolumes.clear();

	int numInputChannels = graph.getMainBusNumInputChannels();
	int numOutputChannels = graph.getMainBusNumOutputChannels();

	graph.disconnectNode(AUDIO_INPUTMIXER_GRAPH_ID);
	inputMixer->setPlayConfigDetails(graph.getMainBusNumInputChannels(), graph.getMainBusNumInputChannels(), currentSampleRate, currentBufferSize);

	graph.disconnectNode(AUDIO_OUTPUTMIXER_GRAPH_ID);
	outputMixer->setPlayConfigDetails(graph.getMainBusNumOutputChannels(), graph.getMainBusNumOutputChannels(), currentSampleRate, currentBufferSize);

	AudioChannelSet inputChannelSet = graph.getChannelLayoutOfBus(true, 0);
	for (int i = 0; i < numInputChannels; ++i)
	{
		String channelName = "Input " + String(i + 1);// AudioChannelSet::getChannelTypeName(inputChannelSet.getTypeOfChannel(i));
		FloatParameter* v = inputVolumesCC.addFloatParameter(channelName + " Gain", "Gain to apply to this input channel", 1, 0, 3);
		inputVolumes.add(v);

		graph.addConnection(AudioProcessorGraph::Connection({ AUDIO_INPUT_GRAPH_ID, i }, { AUDIO_INPUTMIXER_GRAPH_ID, i }));
	}
	inputVolumesCC.loadJSONData(inData);

	AudioChannelSet outputChannelSet = graph.getChannelLayoutOfBus(false, 0);
	for (int i = 0; i < numOutputChannels; ++i)
	{
		String channelName = "Output " + String(i + 1);
		FloatParameter* v = outputVolumesCC.addFloatParameter(channelName + " Gain", "Gain to apply to this input channel", 1, 0, 3);
		outputVolumes.add(v);

		graph.addConnection(AudioProcessorGraph::Connection({ AUDIO_OUTPUTMIXER_GRAPH_ID, i }, { AUDIO_OUTPUT_GRAPH_ID, i }));

	}
	outputVolumesCC.loadJSONData(outData);

	audioModuleListeners.call(&AudioModuleListener::audioSetupChanged);
	audioModuleListeners.call(&AudioModuleListener::monitorSetupChanged);

	if (setup.outputDeviceName.isEmpty()) setWarningMessage("Module is not connected to an audio output");
	else clearWarning();

	am.addAudioCallback(&player);
	am.addAudioCallback(this);

	graph.suspendProcessing(false);
}

void AudioModule::updateSelectedMonitorChannels()
{
	selectedMonitorOutChannels.clear();
	for (int i = 0; i < monitorOutChannels.size(); ++i)
	{
		if (monitorOutChannels[i]->boolValue())
		{
			selectedMonitorOutChannels.add(i);
		}
	}

	numActiveMonitorOutputs = selectedMonitorOutChannels.size();
}

void AudioModule::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	Module::onControllableFeedbackUpdateInternal(cc, c);

	if (c->type == Controllable::BOOL && monitorOutChannels.indexOf((BoolParameter*)c) > -1)
	{
		updateSelectedMonitorChannels();
	}
	else if (c == pitchDetectionMethod)
	{
		PitchDetectionMethod pdm = pitchDetectionMethod->getValueDataAsEnum<PitchDetectionMethod>();

		AudioDeviceManager::AudioDeviceSetup s;
		am.getAudioDeviceSetup(s);


		switch (pdm)
		{
		case NONE: pitchDetector.reset(nullptr); break;
		case MPM: pitchDetector.reset(new PitchMPM((int)s.sampleRate, s.bufferSize));  break;
		case YIN: pitchDetector.reset(new PitchYIN((int)s.sampleRate, s.bufferSize)); break;
		}

	}
	else if (c == ltcFPS)
	{
		curLTCFPS = (int)ltcFPS->getValueData();
	}
	else if (c == ltcParamsCC.enabled)
	{
		if (!ltcParamsCC.enabled->boolValue()) ltcPlaying->setValue(false);
	}
}

void AudioModule::onContainerParameterChangedInternal(Parameter* p)
{
	if (p == enabled)
	{
		if (enabled->boolValue()) player.setProcessor(&graph);
		else player.setProcessor(nullptr);
	}
}

var AudioModule::getJSONData()
{
	var data = Module::getJSONData();

	std::unique_ptr<XmlElement> xmlData(am.createStateXml());
	if (xmlData != nullptr)
	{
		data.getDynamicObject()->setProperty("audioSettings", xmlData->toString());
	}

	data.getDynamicObject()->setProperty("analyzer", analyzerManager.getJSONData());
	//data.getDynamicObject()->setProperty("monitorParams", monitorParams.getJSONData());
	//data.getDynamicObject()->setProperty("channelParams", channelParams.getJSONData());

	return data;
}

void AudioModule::loadJSONDataInternal(var data)
{
	if (data.getDynamicObject()->hasProperty("audioSettings"))
	{

		std::unique_ptr<XmlElement> elem = XmlDocument::parse(data.getProperty("audioSettings", ""));
		am.initialise(0, 2, elem.get(), true);
	}

	updateAudioSetup();



	Module::loadJSONDataInternal(data);


	analyzerManager.loadJSONData(data.getProperty("analyzer", var()));
	//monitorParams.loadJSONData(data.getProperty("monitorParams", var()));
	//channelParams.loadJSONData(data.getProperty("channelParams", var()));


	AudioDeviceManager::AudioDeviceSetup setup;
	am.getAudioDeviceSetup(setup);
	if (setup.outputDeviceName.isEmpty()) setWarningMessage("Module is not connected to an audio output");
	else clearWarning();
}

void AudioModule::audioDeviceIOCallbackWithContext(const float* const* inputChannelData,
	int numInputChannels,
	float* const* outputChannelData,
	int numOutputChannels,
	int numSamples,
	const AudioIODeviceCallbackContext& context)
{
	//DBG("audio callback");

	for (int i = 0; i < numOutputChannels; ++i) FloatVectorOperations::clear(outputChannelData[i], numSamples);

	if (!enabled->boolValue()) return;

	for (int i = 0; i < numInputChannels; ++i)
	{
		float channelVolume = i < inputVolumes.size() && inputVolumes[i] != nullptr ? inputVolumes[i]->floatValue() : 1;

		if (i == 0) //take only the first channel for analysis (later, should be able to select which channel is used for analysis)
		{
			if (buffer.getNumSamples() != numSamples) buffer.setSize(1, numSamples);
			buffer.copyFromWithRamp(0, 0, inputChannelData[0], numSamples, 1, inputGain->floatValue() * channelVolume);
			detectedVolume->setValue(buffer.getRMSLevel(0, 0, numSamples));

			if (detectedVolume->floatValue() > activityThreshold->floatValue())
			{
				inActivityTrigger->trigger();

				if (pitchDetector != nullptr)
				{
					if ((int)pitchDetector->getBufferSize() != numSamples) pitchDetector->setBufferSize(numSamples);

					if (inputChannelData[0][0] >= 0)
					{
						float freq = pitchDetector->getPitch(inputChannelData[0]);
						frequency->setValue(freq);
						int pitchNote = getNoteForFrequency(freq);
						pitch->setValue(pitchNote);

						note->setValueWithKey(MIDIManager::getNoteName(pitchNote, false));
						octave->setValue(floor(pitchNote / 12.0));
					}
				}
			}
			else
			{
				if (!keepLastDetectedValues->boolValue())
				{
					frequency->setValue(0);
					pitch->setValue(0);
					note->setValueWithKey("-");
				}
			}
		}



		//Monitor
		if (monitorParams.enabled->boolValue())
		{
			for (int j = 0; j < numActiveMonitorOutputs; j++)
			{
				int outputIndex = selectedMonitorOutChannels[j];
				if (outputIndex >= numOutputChannels) continue;
				FloatVectorOperations::addWithMultiply(outputChannelData[outputIndex], inputChannelData[i], monitorVolume->floatValue() * channelVolume, numSamples);
			}
		}
	}

	//Analysis
	if (numInputChannels > 0)
	{
		analyzerManager.process(inputChannelData[0], numSamples);

		if (ltcParamsCC.enabled->boolValue())
		{
			int channel = ltcChannel->intValue() - 1;
			if (channel >= 0 && channel < numInputChannels)
			{
				ltc_decoder_write_float(ltcDecoder.get(), (float*)inputChannelData[channel], numSamples, 0);

				bool hasLTC = false;
				LTCFrameExt frame;
				while (ltc_decoder_read(ltcDecoder.get(), &frame))
				{
					SMPTETimecode stime;
					ltc_frame_to_time(&stime, &frame.ltc, (ltcUseDate->boolValue() ? 1 : 0));

					float time = stime.days * 3600 * 24 + stime.hours * 3600 + stime.mins * 60 + stime.secs + stime.frame * 1.0f / curLTCFPS;
					ltcTime->setValue(time);
					hasLTC = true;
				}

				if (!hasLTC)
				{
					if (ltcPlaying->boolValue())
					{
						ltcFrameDropCount++;
						if (ltcFrameDropCount >= 10) ltcPlaying->setValue(hasLTC);
					}
				}
				else
				{
					ltcFrameDropCount = 0;
					ltcPlaying->setValue(true);
				}
			}
		}
	}
}

void AudioModule::audioDeviceAboutToStart(AudioIODevice*)
{

}

void AudioModule::audioDeviceStopped()
{
}

void AudioModule::changeListenerCallback(ChangeBroadcaster*)
{
	updateAudioSetup();
}

void AudioModule::itemAdded(FFTAnalyzer* item)
{
	fftCC.addParameter(item->value);
}

void AudioModule::itemsAdded(Array<FFTAnalyzer*> items)
{
	for (auto& item : items) fftCC.addParameter(item->value);
}

void AudioModule::itemRemoved(FFTAnalyzer* item)
{
	fftCC.removeControllable(item->value);
}

void AudioModule::itemsRemoved(Array<FFTAnalyzer*> items)
{
	for (auto& item : items) fftCC.removeControllable(item->value);
}


int AudioModule::getNoteForFrequency(float freq)
{
	return (int)(69 + 12 * log2(freq / 440)); //A = 440
}

AudioModuleHardwareSettings::AudioModuleHardwareSettings(AudioDeviceManager* am) :
	ControllableContainer("Hardware Settings"),
	am(am)
{
}

InspectableEditor* AudioModuleHardwareSettings::getEditorInternal(bool isRoot, Array<Inspectable*> inspectables)
{
	return new AudioModuleHardwareEditor(this, isRoot);
}



// MIXER


MixerProcessor::MixerProcessor(AudioModule* m, bool isInput) :
	AudioProcessor(),
	audioModule(m),
	isInput(isInput)
{
	gains = isInput ? &audioModule->inputVolumes : &audioModule->outputVolumes;
}

void MixerProcessor::prepareToPlay(double sampleRate, int maximumExpectedSamplesPerBlock)
{
}

void MixerProcessor::releaseResources()
{

}

void MixerProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	prevGains.resize(gains->size());

	for (int i = 0; i < buffer.getNumChannels() && i < gains->size(); i++)
	{
		float newGain = gains->getUnchecked(i)->floatValue();
		buffer.applyGainRamp(i, 0, buffer.getNumSamples(), prevGains[i], newGain);
		prevGains.set(i, newGain);
	}
}

double MixerProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

bool MixerProcessor::acceptsMidi() const
{
	return false;
}

bool MixerProcessor::producesMidi() const
{
	return false;
}

AudioProcessorEditor* MixerProcessor::createEditor()
{
	return nullptr;
}

bool MixerProcessor::hasEditor() const
{
	return false;
}

int MixerProcessor::getNumPrograms()
{
	return 0;
}

int MixerProcessor::getCurrentProgram()
{
	return 0;
}

void MixerProcessor::setCurrentProgram(int index)
{
}

const String MixerProcessor::getProgramName(int index)
{
	return String();
}

void MixerProcessor::changeProgramName(int index, const String& newName)
{
}

void MixerProcessor::getStateInformation(juce::MemoryBlock& destData)
{
}

void MixerProcessor::setStateInformation(const void* data, int sizeInBytes)
{
}
